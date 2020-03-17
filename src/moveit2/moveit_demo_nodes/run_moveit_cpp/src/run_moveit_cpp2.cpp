/* Inspired from Henning Kayser, Desc: A simple demo node running MoveItCpp for planning and execution*/
#include <thread>
#include <rclcpp/rclcpp.hpp>
#include <moveit/moveit_cpp/moveit_cpp.h>
#include <moveit/moveit_cpp/planning_component.h>
#include <moveit/robot_state/conversions.h>
#include <moveit_msgs/msg/display_robot_state.hpp>
#include <trajectory_msgs/msg/joint_trajectory.hpp>

static const rclcpp::Logger LOGGER = rclcpp::get_logger("moveit_cpp_demo");

class MoveItCppDemo 
{ 
public:
  MoveItCppDemo(const rclcpp::Node::SharedPtr& node): node_(node), robot_state_publisher_(node_->create_publisher<moveit_msgs::msg::DisplayRobotState>("display_robot_state", 1)), trajectory_publisher_(node_->create_publisher<trajectory_msgs::msg::JointTrajectory>("/fake_joint_trajectory_controller/joint_trajectory", 1)){}

  void run()
  {
    RCLCPP_INFO(LOGGER, "Initialize MoveItCpp");
    moveit_cpp_ = std::make_shared<moveit::planning_interface::MoveItCpp>(node_);
    moveit_cpp_->getPlanningSceneMonitor()->setPlanningScenePublishingFrequency(100);

    RCLCPP_INFO(LOGGER, "Initialize PlanningComponent");
    moveit::planning_interface::PlanningComponent arm("panda_arm", moveit_cpp_);

    // A little delay before running the plan
    rclcpp::sleep_for(std::chrono::seconds(3));

    // ===================== Add box ========================//
    moveit_msgs::msg::CollisionObject collision_object;
    collision_object.header.frame_id = "panda_link0";
    collision_object.id = "box";
    shape_msgs::msg::SolidPrimitive box;
    box.type = box.BOX;
    box.dimensions = { 0.1, 0.4, 0.1 }; //x,y,z
    geometry_msgs::msg::Pose box_pose;
    box_pose.position.x = 0.2;
    box_pose.position.y = 0.0;
    box_pose.position.z = 1.0;
    collision_object.primitives.push_back(box);
    collision_object.primitive_poses.push_back(box_pose);
    collision_object.operation = collision_object.ADD;
    {  // Lock PlanningScene
      planning_scene_monitor::LockedPlanningSceneRW scene(moveit_cpp_->getPlanningSceneMonitor());
      scene->processCollisionObjectMsg(collision_object); // Add object to planning scene
    }  // Unlock PlanningScene

    
    RCLCPP_INFO(LOGGER, " ======== Set goal ======= ");
    arm.setGoal("home");

    RCLCPP_INFO(LOGGER, " ======== Plan to goal ======== ");
    const auto plan_solution = arm.plan();
    if (plan_solution)
    {
      visualizeTrajectory(*plan_solution.trajectory);

      // TODO(henningkayser): Enable trajectory execution once controllers are available
      // RCLCPP_INFO(LOGGER, "arm.execute()");
      // arm.execute();
      // Right now the joint trajectory controller doesn't support actions and the current way to send trajectory is by
      // using a publisher
      // See https://github.com/ros-controls/ros2_controllers/issues/12 for enabling action interface progress
      RCLCPP_INFO(LOGGER, "Sending the trajectory for execution");
      moveit_msgs::msg::RobotTrajectory robot_trajectory;
      plan_solution.trajectory->getRobotTrajectoryMsg(robot_trajectory);
      trajectory_publisher_->publish(robot_trajectory.joint_trajectory);
    }
  }

private:
  void visualizeTrajectory(const robot_trajectory::RobotTrajectory& trajectory)
  {
    moveit_msgs::msg::DisplayRobotState waypoint;
    const auto start_time = node_->now();
    for (size_t i = 0; i < trajectory.getWayPointCount(); ++i)
    {
      moveit::core::robotStateToRobotStateMsg(trajectory.getWayPoint(i), waypoint.state);
      const auto waypoint_time =
          start_time + rclcpp::Duration::from_seconds(trajectory.getWayPointDurationFromStart(i));
      const auto now = node_->now();
      if (waypoint_time > now)
        rclcpp::sleep_for(std::chrono::nanoseconds((waypoint_time - now).nanoseconds()));

      robot_state_publisher_->publish(waypoint);
    }
  }

  rclcpp::Node::SharedPtr node_;
  rclcpp::Publisher<moveit_msgs::msg::DisplayRobotState>::SharedPtr robot_state_publisher_;
  rclcpp::Publisher<trajectory_msgs::msg::JointTrajectory>::SharedPtr trajectory_publisher_;
  moveit::planning_interface::MoveItCppPtr moveit_cpp_;
};

int main(int argc, char** argv)
{
  RCLCPP_INFO(LOGGER, "Initialize node");
  rclcpp::init(argc, argv);
  rclcpp::NodeOptions node_options;
  node_options.automatically_declare_parameters_from_overrides(true);  // This enables loading undeclared parameters best practice would be to declare parameters in the corresponding classes and provide descriptions about expected use
  rclcpp::Node::SharedPtr node = rclcpp::Node::make_shared("run_moveit_cpp", "", node_options);

  MoveItCppDemo demo(node);
  std::thread run_demo(
    [&demo]() {
        rclcpp::sleep_for(std::chrono::seconds(5)); // Let RViz initialize before running demo // TODO(henningkayser): use lifecycle events to launch node
        demo.run();
    }
  );
  rclcpp::spin(node);
  run_demo.join();

  return 0;
}