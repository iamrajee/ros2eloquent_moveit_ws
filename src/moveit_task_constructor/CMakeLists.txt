cmake_minimum_required(VERSION 3.5)
project(moveit_task_constructor)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra) # flag -Wpedantic generate nested namespace warning
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(moveit_core REQUIRED COMPONENTS
  rclcpp
  moveit_core
  moveit_ros_planning 
  moveit_msgs
  moveit_ros_planning_interface
  rosidl_default_runtime
)

include_directories(
  include
  ${moveit_core_INCLUDE_DIRS}
  ${moveit_ros_planning_INCLUDE_DIRS}
  ${moveit_ros_planning_interface_INCLUDE_DIRS}
  ${moveit_msgs_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}_subtasks
  src/subtasks/move.cpp
  src/subtasks/current_state.cpp
)

add_library(${PROJECT_NAME}
  src/subtask.cpp
  src/task.cpp
)

add_executable(test_plan_current_state src/test/test_plan_current_state.cpp)
target_link_libraries(test_plan_current_state ${PROJECT_NAME}_subtasks ${PROJECT_NAME})

add_executable(plan_pick src/plan_pick.cpp)
target_link_libraries(plan_pick ${PROJECT_NAME}_subtasks ${PROJECT_NAME})

add_executable(cpp_node src/cpp_node.cpp)
target_include_directories(cpp_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

install(TARGETS cpp_node test_plan_current_state plan_pick #subtask
  EXPORT export_${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
