#include <rclcpp/rclcpp.hpp>
#include <moveit_task_constructor/task.h>

#include <moveit_task_constructor/subtasks/current_state.h>
#include <moveit_task_constructor/subtasks/gripper.h>
#include <moveit/move_group_interface/move_group_interface.h>

using namespace moveit::task_constructor;

int main(int argc, char** argv){
    rclcpp::init(argc, argv);
    std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("plan_pick"); //std::shared_ptr<int> foo = std::make_shared<int> (10);

	Task t;

	t.addStart( std::make_shared<subtasks::CurrentState>("current state") );

	{
		auto move= std::make_shared<subtasks::Gripper>("open gripper");
		move->setGroup("gripper");
		move->setTo("open");
		t.addAfter(move);
	}
/*
	{
		auto move= std::make_shared<subtasks::Move>("move to pre-grasp");
		move->setGroup("arm");
		move->setPlannerId("RRTConnectkConfigDefault");
		move->setPlanningTime(5.0);
		t.addAfter(move);
	}

	{
		auto move= std::make_shared<subtasks::CartesianPositionMotion>("proceed to grasp pose");
		move->minMaxDistance(3.0, 10.0);
		move->towards("object", 0.0, 0.0, 0.0);
		t.addAfter(move);
	}

	{
		auto gengrasp= std::make_shared<GenerateGraspPose>("generate grasp pose");
		gengrasp->setEndEffector("s_model_tool0");
		gengrasp->setGroup("arm");
		gengrasp->setObject("object");
	}

	{
		auto move= std::make_shared<subtasks::Gripper>("grasp");
		move->setGroup("gripper");
		move->setTo("closed");
		move->graspObject("object");
		t.addAfter(move);
	}

	{
		auto move= std::make_shared<subtasks::CartesianPositionMotion>("Lift Object");
		move->setGroup("arm");
		move->minMaxDistance(3.0, 5.0);
		move->along("world", 0.0, 0.0, 1.0);
		t.addAfter(move);
	}
*/
	t.plan();

	t.printState();

	rclcpp::spin(node);
	rclcpp::shutdown();
	return 0;
}
