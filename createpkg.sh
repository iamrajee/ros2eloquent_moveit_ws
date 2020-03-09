#!/bin/bash

cd src/
ros2 pkg create $1 --build-type ament_cmake --node-name cpp_node
cd ../
#ls src/
make
source refresh.sh
#source /opt/ros/eloquent/setup.bash
#source install/local_setup.bash
#source install/setup.bash
#clear

