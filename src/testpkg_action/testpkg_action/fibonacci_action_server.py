import time
import rclpy
from rclpy.action import ActionServer
from rclpy.node import Node
# from testpkg_action.action import Fibonacci
# from testpkg_action import *
# from testpkg_action import Fibonacci
# from testpkg_action.action import *
# from action import *
# from testpkg_action.action import *

# import sys
# # sys.path.append('/home/rajendra/ros2eloquent_catkin_ws/src/testpkg_action/action')
# sys.path.append('/home/rajendra/ros2eloquent_catkin_ws/src/testpkg_action/')
# # import Fibonacci
# from action import *
# # import Fibonacci.action

from action_tutorials_interfaces.action import Fibonacci

class FibonacciActionServer(Node):
    def __init__(self):
        super().__init__('fibonacci_action_server')
        self._action_server = ActionServer(
            self,
            Fibonacci,
            'fibonacci',
            self.execute_callback)

    def execute_callback(self, goal_handle):
        self.get_logger().info('Executing goal...')
        # return Fibonacci.Result()
        feedback_msg = Fibonacci.Feedback()
        feedback_msg.partial_sequence = [0, 1]

        for i in range(1, goal_handle.request.order):
            feedback_msg.partial_sequence.append(
                feedback_msg.partial_sequence[i] + feedback_msg.partial_sequence[i-1])
            self.get_logger().info('Feedback: {0}'.format(feedback_msg.partial_sequence))
            goal_handle.publish_feedback(feedback_msg)
            time.sleep(1)

        goal_handle.succeed()

        result = Fibonacci.Result()
        result.sequence = feedback_msg.partial_sequence
        return result

def main(args=None):
    rclpy.init(args=args)
    fibonacci_action_server = FibonacciActionServer()
    # rclpy.spin(fibonacci_action_server)
    try:
        rclpy.spin(fibonacci_action_server)
    except KeyboardInterrupt:
        pass

if __name__ == '__main__':
    main()