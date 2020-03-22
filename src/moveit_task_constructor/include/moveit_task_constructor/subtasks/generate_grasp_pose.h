// copyright Michael 'v4hn' Goerner @ 2017

#pragma once

#include <moveit_task_constructor/subtask.h>

#include <rclcpp/rclcpp.hpp>

#include <moveit/macros/class_forward.h>
#include <moveit_msgs/msg/display_robot_state.hpp> //added new

namespace moveit::planning_interface {
MOVEIT_CLASS_FORWARD(MoveGroupInterface);
}

namespace moveit::task_constructor::subtasks {

class GenerateGraspPose : public SubTask {
public:
	GenerateGraspPose(std::string name);

	virtual bool canCompute();

	virtual bool compute();

	void setGroup(std::string group_name);

	void setEndEffector(std::string eef_link);

	void setObject(std::string object);

	void setTimeout(double timeout);

protected:
	std::string group_;

	std::string eef_;

	std::string object_;

	double timeout_;

   /* temp values */
	double current_angle_;

	double remaining_time_;

	rclcpp::Publisher<moveit_msgs::msg::DisplayRobotState>::SharedPtr pub; //ros::Publisher pub;
	
	
};

}