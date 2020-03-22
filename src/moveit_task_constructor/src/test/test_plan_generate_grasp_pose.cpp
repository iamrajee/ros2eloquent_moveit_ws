#include <moveit_task_constructor/task.h>
#include <moveit_task_constructor/subtasks/generate_grasp_pose.h>

#include <rclcpp/rclcpp.hpp>
#include <moveit_msgs/msg/collision_object.hpp>
#include <shape_msgs/msg/solid_primitive.hpp>

#include <moveit/planning_scene_interface/planning_scene_interface.h>

using namespace moveit::task_constructor;

void spawnObject(std::shared_ptr<rclcpp::Node>& node){
	moveit::planning_interface::PlanningSceneInterface psi(node); // moveit::planning_interface::PlanningSceneInterface psi;

	moveit_msgs::msg::CollisionObject o;
	o.id= "object";
	o.header.frame_id= "world";
	o.primitive_poses.resize(1);
	o.primitive_poses[0].position.x= 0.53;
	o.primitive_poses[0].position.y= 0.55;
	o.primitive_poses[0].position.z= 0.84;
	o.primitive_poses[0].orientation.w= 1.0;
	o.primitives.resize(1);
	o.primitives[0].type= shape_msgs::msg::SolidPrimitive::SPHERE;
	o.primitives[0].dimensions.resize(1, 0.03);
	psi.applyCollisionObject(o);
}


int main(int argc, char** argv){
	rclcpp::init(argc, argv);
    std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("test_plan_generate_grasp_pose");
	// ros::AsyncSpinner spinner(1);
	// spinner.start();

	spawnObject(node);

	Task t;

	auto st= std::make_shared<subtasks::GenerateGraspPose>("generate grasp candidates");

	st->setEndEffector("s_model_tool0");
	st->setGroup("arm");
	st->setObject("object");
	st->setTimeout(0.5);
	st->setAngleDelta(0.1);
	st->setGraspOffset(0.03);
	

	t.addStart(st);

	t.plan();

	t.printState();

	// rclcpp::spin(node);

	return 0;
}
