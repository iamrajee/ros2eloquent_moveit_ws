#include <moveit_task_constructor/task.h>
#include <moveit_task_constructor/subtask.h>
#include <moveit/robot_model_loader/robot_model_loader.h> 
#include <rclcpp/rclcpp.hpp>
#include <rclcpp/client.hpp>

#include <moveit_msgs/srv/get_planning_scene.hpp>

moveit::task_constructor::Task::Task(){
	rml_.reset(); // rml_.reset(new robot_model_loader::RobotModelLoader);
	if( !rml_->getModel() )	
	throw Exception("Task failed to construct RobotModel");

	std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("get_planning_scene_node");
	rclcpp::Client<moveit_msgs::srv::GetPlanningScene>::SharedPtr client = node->create_client<moveit_msgs::srv::GetPlanningScene>("get_planning_scene");
	client->wait_for_service(); //client.waitForExistence();

	moveit_msgs::srv::GetPlanningScene::Request req; //std::shared_ptr<moveit_msgs::srv::GetPlanningScene::Request> req = std::make_shared<moveit_msgs::srv::GetPlanningScene::Request> ();
	moveit_msgs::srv::GetPlanningScene::Response res;
	
	req.components.components =
		  moveit_msgs::msg::PlanningSceneComponents::SCENE_SETTINGS
		| moveit_msgs::msg::PlanningSceneComponents::ROBOT_STATE
		| moveit_msgs::msg::PlanningSceneComponents::ROBOT_STATE_ATTACHED_OBJECTS
		| moveit_msgs::msg::PlanningSceneComponents::WORLD_OBJECT_NAMES
		| moveit_msgs::msg::PlanningSceneComponents::WORLD_OBJECT_GEOMETRY
		| moveit_msgs::msg::PlanningSceneComponents::OCTOMAP
		| moveit_msgs::msg::PlanningSceneComponents::TRANSFORMS
		| moveit_msgs::msg::PlanningSceneComponents::ALLOWED_COLLISION_MATRIX
		| moveit_msgs::msg::PlanningSceneComponents::LINK_PADDING_AND_SCALING
		| moveit_msgs::msg::PlanningSceneComponents::OBJECT_COLORS;
	
	// if(!client.call(req, res)){
		// throw Exception("Task failed to aquire current PlanningScene");
	// }
	// client->async_send_request(req); //new
	scene_.reset(new planning_scene::PlanningScene(rml_->getModel()));
	scene_->setPlanningSceneMsg(res.scene);
}

moveit::task_constructor::Task::~Task(){
	subtasks_.clear();
	scene_.reset();
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
		std::cout << "Computing subtask '" << subtask->getName() << "': " << std::endl;
		bool success= subtask->compute();
		std::cout << (success ? "succeeded" : "failed") << std::endl;
	}
	return false;
}

void moveit::task_constructor::Task::addSubTask( SubTaskPtr subtask ){
	subtask->setPlanningScene( scene_ );
	subtasks_.push_back( subtask );
}

void moveit::task_constructor::Task::printState(){
	for( auto& st : subtasks_ ){
		std::cout
			<< st->getBegin().size() << " -> "
			<< st->getTrajectories().size()
			<< " <- " << st->getEnd().size()
			<< " / " << st->getName()
			<< std::endl;
	}
}