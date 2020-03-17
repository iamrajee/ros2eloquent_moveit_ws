//#include <rcl/rcl.h>
#include <rclcpp/rclcpp.hpp>
// #include <rclcpp/context.hpp> //tried including some extra library but with no luck
// #include <rclcpp/contexts/default_context.hpp>
// #include <rclcpp/init_options.hpp>
// #include <rclcpp/node.hpp>
// #include <rcutils/allocator.h>

#include <chrono>
#include <cstdlib>
#include <memory>

#include <moveit_task_constructor/task.h>
#include <moveit_task_constructor/subtask.h>

#include <moveit_task_constructor/subtasks/current_state.h>

using namespace moveit::task_constructor;

int main(int argc, char** argv){
	//rclcpp::init(argc, argv,const rclcpp::InitOptions & init_options = rclcpp::InitOptions()); //gave linker error
	rclcpp::init(argc, argv); //gave linker error
	Task t;

	t.addStart( std::make_shared<subtasks::CurrentState>("current state") );

	t.plan();

	rclcpp::shutdown(); //gave linker error
	return 0;
}