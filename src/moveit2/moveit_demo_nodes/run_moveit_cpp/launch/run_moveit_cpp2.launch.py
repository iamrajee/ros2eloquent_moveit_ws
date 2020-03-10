import os
import yaml
from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

# Helper function to load file
def load_file(package_name, file_path):
    package_path = get_package_share_directory(package_name)
    absolute_file_path = os.path.join(package_path, file_path)
    try:
        with open(absolute_file_path, 'r') as file:
            return file.read()
    except EnvironmentError: # parent of IOError, OSError *and* WindowsError where available
        return None

# Helper function to load yaml
def load_yaml(package_name, file_path):
    package_path = get_package_share_directory(package_name)
    absolute_file_path = os.path.join(package_path, file_path)
    try:
        with open(absolute_file_path, 'r') as file:
            return yaml.load(file)
    except EnvironmentError: # parent of IOError, OSError *and* WindowsError where available
        return None


def generate_launch_description(): #main funtion

    # ========= load files(yaml,urdf,srdf,rviz) ========= #
    rviz_config_file = get_package_share_directory('run_moveit_cpp') + "/launch/run_moveit_cpp2.rviz"
    moveit_cpp_yaml_file_name = get_package_share_directory('run_moveit_cpp') + "/config/moveit_cpp2.yaml" ### ---- moveit_cpp.yaml is passed by filename for now since it's node specific
    panda_controllers_yaml = os.path.join(get_package_share_directory("run_moveit_cpp"), "config", "panda_controllers2.yaml")
    start_positions_yaml = os.path.join(get_package_share_directory("run_moveit_cpp"), "config", "start_positions2.yaml")
    controllers_yaml = load_yaml('run_moveit_cpp', 'config/controllers2.yaml')
    robot_description_config = load_file('moveit_resources', 'panda_description/urdf/panda.urdf') # Component yaml files are grouped in separate namespaces
    robot_description_semantic_config = load_file('moveit_resources', 'panda_moveit_config/config/panda.srdf')
    kinematics_yaml = load_yaml('moveit_resources', 'panda_moveit_config/config/kinematics.yaml')
    ompl_planning_yaml = load_yaml('moveit_resources', 'panda_moveit_config/config/ompl_planning.yaml')

    # ======== params ======== #
    robot_description = {'robot_description' : robot_description_config}
    robot_description_semantic = {'robot_description_semantic' : robot_description_semantic_config}
    robot_description_kinematics = { 'robot_description_kinematics' : kinematics_yaml }
    moveit_controllers = { 'moveit_simple_controller_manager' : controllers_yaml }
    ompl_planning_pipeline_config = { 'ompl' : { 'planning_plugin' : 'ompl_interface/OMPLPlanner', 'request_adapters' : """default_planner_request_adapters/AddTimeOptimalParameterization default_planner_request_adapters/FixWorkspaceBounds default_planner_request_adapters/FixStartStateBounds default_planner_request_adapters/FixStartStateCollision default_planner_request_adapters/FixStartStatePathConstraints""" , 'start_state_max_bounds_error' : 0.1 } }
    ompl_planning_pipeline_config['ompl'].update(ompl_planning_yaml)

    # ========= Nodes ========= #
    # --------- run_moveit_cpp node --------- 
    run_moveit_cpp_node = Node(node_name='run_moveit_cpp', package='run_moveit_cpp', node_executable='run_moveit_cpp2', output='screen', parameters=[moveit_cpp_yaml_file_name, robot_description, robot_description_semantic, kinematics_yaml, ompl_planning_pipeline_config, moveit_controllers]) #TODO(henningkayser): add debug argument #prefix='xterm -e gdb --args',
    # --------- RViz ---------
    rviz_node = Node(package='rviz2', node_executable='rviz2', node_name='rviz2', output='log', arguments=['-d', rviz_config_file], parameters=[robot_description])
    # --------- static TF ---------
    static_tf = Node(package='tf2_ros', node_executable='static_transform_publisher', node_name='static_transform_publisher', output='log', arguments=['0.0', '0.0', '0.0', '0.0', '0.0', '0.0', 'world', 'panda_link0'])
    # --------- Fake joint driver ---------   
    fake_joint_driver_node = Node(package='fake_joint_driver', node_executable='fake_joint_driver_node', parameters=[panda_controllers_yaml, start_positions_yaml, robot_description] ) ###TODO(JafarAbdi): Why this launch the two nodes (controller manager and the fake joint driver) with the same name! # node_name='fake_joint_driver_node',

    return LaunchDescription( [static_tf, rviz_node, run_moveit_cpp_node, fake_joint_driver_node] )