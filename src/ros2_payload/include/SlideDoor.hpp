/*
  Created by : youliang
  Created on : 25-09-2018
  Description: 
*/

#ifndef SLIDEDOOR_HPP
#define SLIDEDOOR_HPP

#include <iostream>
#include <unistd.h>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int8.hpp"
#include <signal.h>

#ifndef WIRINGPI
//#define WIRINGPI
#endif

#ifdef WIRINGPI
#include <wiringPi.h>
#endif

using namespace std;
using std::placeholders::_1;

#define AIN1 3 //Direction 1
#define AIN2 4 //Direction 2
#define PWMA 1 //control the speed
#define LSw1 0 //Limit switch 1
#define LSw2 2 //Limit switch 2
#define LSw3 5 //Limit switch 3

class MotorController : public rclcpp::Node
{
  public:
    MotorController();
    virtual ~MotorController();
    void door_control_command_callback(const std_msgs::msg::Int8::SharedPtr msg);
    void gpio_setup();
    void rotate(int speed, int direction);
    void loop();
  public:
    int command = -1;
    int motor_direction = -1;
    bool received_interrupt = true;
    rclcpp::Subscription<std_msgs::msg::Int8>::SharedPtr door_num_subscription_;
    rclcpp::TimerBase::SharedPtr sub_timer_;
};

#endif
