/*
  Created by : YouLiang
  Created on : 08-11-2018
  Description: 
*/

#include <SlideDoor.hpp>

using namespace std;
using std::placeholders::_1;

MotorController::MotorController() : Node("motor_controller_wrap")
{
    RCLCPP_INFO(this->get_logger(), "Starting now: ");
    gpio_setup();
    door_num_subscription_ = this->create_subscription<std_msgs::msg::Int8>("door_control_command", 10, std::bind(&MotorController::door_control_command_callback, this, _1));
    sub_timer_ = this->create_wall_timer(500ms, std::bind(&MotorController::loop, this));
}
MotorController::~MotorController() {}

void MotorController::door_control_command_callback(const std_msgs::msg::Int8::SharedPtr msg)
{
    command = msg->data;
}

void MotorController::gpio_setup()
{
    RCLCPP_INFO(this->get_logger(), "Setting up GPIO");
#ifdef WIRINGPI
    if (wiringPiSetup() == -1)
    {
        cout << "WiringPi setup failed" << endl;
    }
    cout << "WiringPi successfully setup, setting pinMode" << endl;
    pinMode(PWMA, PWM_OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(LSw1, INPUT);
    pullUpDnControl(LSw1, PUD_UP);
    pinMode(LSw2, INPUT);
    pullUpDnControl(LSw2, PUD_UP);
    pinMode(LSw3, INPUT);
    pullUpDnControl(LSw3, PUD_UP);
#endif
}

void MotorController::rotate(int speed, int direction)
{
    RCLCPP_INFO(this->get_logger(), "inside rotate function ");
    //direction: 1 clockwise, 0 counter-clockwise
#ifdef WIRINGPI
    bool inPin1 = LOW;
    bool inPin2 = HIGH;

    if (direction == 1)
    {
        cout << "turning counter-clockwise" << endl;
        inPin1 = HIGH;
        inPin2 = LOW;
    }
    else
    {
        cout << "turning clockwise" << endl;
        inPin1 = LOW;
        inPin2 = HIGH;
    }

    digitalWrite(AIN1, inPin1);
    digitalWrite(AIN2, inPin2);
    pwmWrite(PWMA, speed);
#endif
speed = speed; //to avoid unsed warnnings
direction = direction; //to avoid unsed warnnings
}

void MotorController::loop()
{
    RCLCPP_INFO(this->get_logger(), "Inside loop function: command = '%d'", command);
    switch (command)
    {
        //Open left side of the payload
    case 1:
    {
        RCLCPP_INFO(this->get_logger(), "received 1");
#ifdef WIRINGPI
        if ((digitalRead(LSw1) == LOW)) //Not pushed is LOW
        {
            cout << "Rotating clockwise.. Opening left side" << endl;
            motor_direction = 1;
            rotate(400, motor_direction);
            sleep(0.2);
        }
        else if (received_interrupt)
        {

            rotate(400, motor_direction);
            sleep(0.2);
        }
        else
        {
            received_interrupt = false;
            cout << "Already at left open" << endl;
            pwmWrite(PWMA, 0);
        }
#endif
        break;
    }
        //Fully close the door of the payload
    case 2:
    {
        RCLCPP_INFO(this->get_logger(), "received 2");
#ifdef WIRINGPI
        if ((digitalRead(LSw1) == HIGH))
        {
            cout << "Rotating counter-clockwise.. Fully closing the door" << endl;
            motor_direction = 0;
            rotate(400, motor_direction);
            sleep(0.2);
        }
        else if ((digitalRead(LSw3) == HIGH))
        {
            cout << "Rotating clockwise.. Fully closing the door" << endl;
            motor_direction = 1;
            rotate(400, motor_direction);
            sleep(0.2);
        }
        else if ((digitalRead(LSw2) == HIGH))
        {
            cout << "Already fully closed" << endl;
            received_interrupt = false;
            pwmWrite(PWMA, 0);
        }
        else if (received_interrupt)
        {
            rotate(400, motor_direction);
            sleep(0.2);
        }
        else
        {
            cout << "Rotating..." << endl;
        }
#endif
        break;
    }
        //Open right side of the payload
    case 3:
    {
        RCLCPP_INFO(this->get_logger(), "received 3");
#ifdef WIRINGPI
        if ((digitalRead(LSw3) == LOW))
        {
            cout << "Rotating counter-clockwise.. Opening right side" << endl;
            motor_direction = 0;
            rotate(400, motor_direction);
            sleep(0.2);
        }
        else if (received_interrupt)
        {
            rotate(400, motor_direction);
            sleep(0.2);
        }
        else
        {
            received_interrupt = false;
            cout << "Already at right open" << endl;
            pwmWrite(PWMA, 0);
        }
#endif
        break;
    }
    case -1:
    {
        received_interrupt = true;
#ifdef WIRINGPI
        cout << "Encoutered an interrupt" << endl;
        pwmWrite(PWMA, 0);
#endif
        break;
    }
    default:
        cout << "Need to check the code" << endl;
    }
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MotorController>());
    rclcpp::shutdown();
    return 0;
}
