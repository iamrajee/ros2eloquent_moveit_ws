/*
  Created by : You Liang
  Created on : 28-08-2018
*/
#include <chrono>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int8.hpp"
 using namespace std;
 using namespace std::chrono_literals;
 /* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */
 class AGVPublisher : public rclcpp::Node
{
public:
  AGVPublisher()
  : Node("agv_publisher"), count_(1)
  {
    publisher_ = this->create_publisher<std_msgs::msg::Int8>("door_control_command",10);
    timer_ = this->create_wall_timer(
      1500ms, std::bind(&AGVPublisher::timer_callback, this));
  }
 private:
  void timer_callback()
  {
    auto message = std_msgs::msg::Int8();
    cout << "Hey AGV, send door command" << endl;
    cin >> count_;
    // if ((count_!=0) && (count_!=1))
    // {
    //   RCLCPP_INFO(this->get_logger(), "Bad choice :( Please enter 0 or 1")
    //   cin >> count_;   
    // }
    // if (count_ == 1)
    // {
    //   message.data = count_;
    //   count_--;
    // }
    // else 
    // {
    //   count_ =0;
    //   message.data = count_;
    //   count_++;
    // }
    message.data = count_;
    RCLCPP_INFO(this->get_logger(), "Publishing: '%d'", message.data);
    publisher_->publish(message);
  }
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::Int8>::SharedPtr publisher_;
  int count_;
};
 int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<AGVPublisher>());
  rclcpp::shutdown();
  return 0;
} 
