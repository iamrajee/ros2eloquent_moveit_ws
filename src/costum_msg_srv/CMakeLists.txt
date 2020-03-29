cmake_minimum_required(VERSION 3.5)
project(costum_msg_srv)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/Num.msg"
  "srv/AddThreeInts.srv"
 )

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(costum_msg_srv REQUIRED)                         # CHANGE

add_executable(talker src/publisher_member_function.cpp)
ament_target_dependencies(talker rclcpp costum_msg_srv)         # CHANGE

add_executable(listener src/subscriber_member_function.cpp)
ament_target_dependencies(listener rclcpp costum_msg_srv)     # CHANGE

add_executable(server src/add_three_ints_server.cpp)                    #<========== new
ament_target_dependencies(server rclcpp costum_msg_srv)

add_executable(client src/add_three_ints_client.cpp)                    #<========== new
ament_target_dependencies(client rclcpp costum_msg_srv)

install(TARGETS server client DESTINATION lib/${PROJECT_NAME})        #<========== new

install(TARGETS
  talker
  listener
  server
  client
  DESTINATION lib/${PROJECT_NAME})

  
ament_package()
