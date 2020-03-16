#include <rclcpp/rclcpp.hpp>

#include <chrono>
#include <cstdlib>
#include <memory>

#include <moveit_task_constructor/task.h>
#include <moveit_task_constructor/subtask.h>

#include <moveit_task_constructor/subtasks/current_state.h>

using namespace moveit::task_constructor;

int main(int argc, char** argv){
	rclcpp::init(argc, argv);

	Task t;

	t.addStart( std::make_shared<subtasks::CurrentState>("current state") );

	t.plan();

	rclcpp::shutdown();
	return 0;
}