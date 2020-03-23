// copyright Michael 'v4hn' Goerner @ 2017

#pragma once

#include <moveit_task_constructor/subtask.h>

#include <rclcpp/rclcpp.hpp>

#include <geometry_msgs/msg/vector3_stamped.hpp>
#include <geometry_msgs/msg/point_stamped.hpp>
#include <moveit_msgs/msg/display_trajectory.hpp>

namespace moveit::planning_interface {
MOVEIT_CLASS_FORWARD(MoveGroupInterface);
}

namespace moveit::task_constructor::subtasks {

class CartesianPositionMotion : public SubTask {
public:
	CartesianPositionMotion(std::string name);

	virtual bool canCompute();

	virtual bool compute();

	void setGroup(std::string group);
	void setLink(std::string link);

	void setMinDistance(double distance);
	void setMaxDistance(double distance);
	void setMinMaxDistance(double min_distance, double max_distance);

	void towards(geometry_msgs::msg::PointStamped goal);
	void along(geometry_msgs::msg::Vector3Stamped direction);

protected:
	std::string group_;

	std::string link_;

	double min_distance_;
	double max_distance_;

	enum {
		MODE_ALONG= 1,
		MODE_TOWARDS= 2
	} mode_;

	geometry_msgs::msg::PointStamped towards_;
	geometry_msgs::msg::Vector3Stamped along_;

	rclcpp::Publisher<moveit_msgs::msg::DisplayTrajectory>::SharedPtr pub; //ros::Publisher pub;

	bool _computeFromEnding();
	void _publishTrajectory(robot_trajectory::RobotTrajectory& trajectory, const moveit::core::RobotState& start); //changed removed const
};

}
