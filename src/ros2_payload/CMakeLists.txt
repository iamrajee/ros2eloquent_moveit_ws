cmake_minimum_required(VERSION 3.5)
project(ros2_payload)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -lwiringPi -lwiringPiDev")
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(builtin_interfaces REQUIRED)
# find_package(wiringPi REQUIRED)
find_package(rosidl_default_generators REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

include_directories(include)
# include_directories(include ${WIRINGPI_INCLUDE_DIRS})

ament_python_install_package(scripts/)

add_executable(slide_door_wrap src/SlideDoor.cpp)
ament_target_dependencies(slide_door_wrap rclcpp std_msgs)

add_executable(mock_publisher src/MockPublisher.cpp)
ament_target_dependencies(mock_publisher rclcpp std_msgs)


install(TARGETS
  mock_publisher
  slide_door_wrap
  DESTINATION lib/${PROJECT_NAME}
)

install(PROGRAMS
  scripts/PayloadController.py
  DESTINATION bin
)

ament_package()
