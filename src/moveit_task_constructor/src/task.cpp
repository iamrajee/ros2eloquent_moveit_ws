#include <moveit_task_constructor/task.h>
#include <moveit_task_constructor/subtask.h>

// #include <moveit/robot_model_loader/robot_model_loader.h>

// #include <moveit/moveit_cpp/moveit_cpp.h>
// #include <moveit/moveit_cpp/planning_component.h>
#include <moveit/robot_state/conversions.h>
#include <moveit_msgs/msg/display_robot_state.hpp>
#include <trajectory_msgs/msg/joint_trajectory.hpp>

#include "/home/rajendra/ros2eloquent_moveit_ws/src/moveit2/moveit_ros/planning/rdf_loader/include/moveit/rdf_loader/rdf_loader.h"
#include "/home/rajendra/ros2eloquent_moveit_ws/src/moveit2/moveit_ros/planning/kinematics_plugin_loader/include/moveit/kinematics_plugin_loader/kinematics_plugin_loader.h"
// #include <moveit/robot_model_loader/robot_model_loader.h>
#include "/home/rajendra/ros2eloquent_moveit_ws/src/moveit2/moveit_ros/planning/robot_model_loader/include/moveit/robot_model_loader/robot_model_loader.h"

moveit::task_constructor::Task::Task()
{
	// rml_.reset(new robot_model_loader::RobotModelLoader);
	// if( !rml_->getModel() )
	// throw Exception("Task failed to construct RobotModel");
}

void moveit::task_constructor::Task::addStart( SubTaskPtr subtask ){
	subtasks_.clear();
	addSubTask( subtask );
}

void moveit::task_constructor::Task::addAfter( SubTaskPtr subtask ){
	subtask->addPredecessor( subtasks_.back() );
	subtasks_.back()->addSuccessor( subtask );
	addSubTask( subtask );
}

bool moveit::task_constructor::Task::plan(){
	for( SubTaskPtr& subtask : subtasks_ ){
		std::cout << "Computing subtask '" << subtask->getName() << "': ";
		bool success= subtask->compute();
		std::cout << (success ? "succeeded" : "failed") << std::endl;
	}
	return false;
}

void moveit::task_constructor::Task::addSubTask( SubTaskPtr subtask ){
	subtask->setRobotModel( rml_->getModel() );
	subtasks_.push_back( subtask );
}