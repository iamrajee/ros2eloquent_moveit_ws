#!/usr/bin/env python3

# license removed for brevity
# This scripts runs a node that perform as a payload manager task
# Created by Tan You Liang, at Nov 2018

import rclpy
from rclpy.node import Node
import time
import _thread

from std_msgs.msg import Int32


class PayloadControllerNode(Node):
    def __init__(self):
        super().__init__('payload_controller_node')
        self.pub = self.create_publisher(Int32, 'door_control_command', 10)
        self.sub = self.create_subscription(Int32, 'door_status', self.doorStatus_callback, 10)
        self.continue_reading = True

        try:
            _thread.start_new_thread(self.readRFID, ())
        except:
            print("Error in thread creation!!!")
    
    def main(self):
        try:
            rclpy.spin(self)
        except KeyboardInterrupt:  # this happens after Ctrl+C
            pass

        print("starting shutdown...")
        self.continue_reading = False



    def doorStatus_callback(self, msg):
        print("door current state: {}".format(msg.data))

    # Test Thread
    def readRFID(self):
        print("Read rfid thread")
        count = 0
        while self.continue_reading:
            print ("running...")
            msg = Int32()
            msg.data = count
            self.pub.publish(msg)
            time.sleep(1)
            count = count + 1


######################################################################
def payload_controller_main(args=None):
    rclpy.init(args=args)
    node = PayloadControllerNode()
    node.main()


if  __name__ == "__main__":
    payload_controller_main()
