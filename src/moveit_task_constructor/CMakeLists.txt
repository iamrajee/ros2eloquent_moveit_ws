cmake_minimum_required(VERSION 3.5)
project(moveit_task_constructor)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

# if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#   add_compile_options(-Wall) # flag -Wextra)  -Wpedantic generate nested namespace warning
# endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(moveit_ros_planning REQUIRED) #important call it seperately
find_package(moveit_ros_move_group REQUIRED)
find_package(moveit_ros_planning_interface REQUIRED)
find_package(moveit_msgs REQUIRED)
# find_package(rclcpp REQUIRED)
find_package(rcutils REQUIRED)

find_package(Boost COMPONENTS system filesystem REQUIRED) # !!! important add COMPONENTS ...etc to remove "unreference to" error
find_package(moveit_core REQUIRED COMPONENTS
  # Boost 
  moveit_core
  moveit_msgs
  rosidl_default_runtime
  # rcutils
)

include_directories(
  include
  ${ament_cmake_INCLUDE_DIRS}
  ${rclcpp_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${moveit_core_INCLUDE_DIRS}
  ${moveit_msgs_INCLUDE_DIRS}
  ${moveit_ros_planning_INCLUDE_DIRS} #important
  ${moveit_ros_planning_interface_INCLUDE_DIRS}
  ${moveit_msgs_INCLUDE_DIRS}
  ${rcutils_INCLUDE_DIRS}
  ${moveit_ros_move_group_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}_subtasks
  src/subtasks/move.cpp
  src/subtasks/current_state.cpp
  src/subtasks/gripper.cpp
)

add_library(${PROJECT_NAME}
  src/subtask.cpp
  src/task.cpp
)

set(dep_list 
  "rclcpp"
  "moveit_msgs"
  "moveit_ros_planning"
  "moveit_ros_planning_interface"
  "Boost"
  "rosidl_default_runtime"
  "moveit_core"
  "ament_cmake"
  "rcutils"
  "moveit_ros_move_group"
)
ament_target_dependencies(${PROJECT_NAME}_subtasks ${dep_list})
ament_target_dependencies(${PROJECT_NAME} ${dep_list}) #!!! important both rclcpp and moveit_msgs



add_executable(plan_pick src/plan_pick.cpp)
ament_target_dependencies(plan_pick ${dep_list})
target_link_libraries(plan_pick ${PROJECT_NAME}_subtasks ${PROJECT_NAME})

add_executable(test_plan_current_state src/test/test_plan_current_state.cpp)
ament_target_dependencies(test_plan_current_state ${dep_list}) #!!! important both rclcpp to avoid unrefernced error
target_link_libraries(test_plan_current_state ${PROJECT_NAME}_subtasks ${PROJECT_NAME})

add_executable(test_plan_gripper src/test/test_plan_gripper.cpp)
ament_target_dependencies(test_plan_gripper ${dep_list})
target_link_libraries(test_plan_gripper ${PROJECT_NAME}_subtasks ${PROJECT_NAME})



add_executable(cpp_node src/cpp_node.cpp)
target_include_directories(cpp_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

install(TARGETS cpp_node test_plan_current_state plan_pick test_plan_gripper
  EXPORT export_${PROJECT_NAME}
  DESTINATION lib/${PROJECT_NAME})

# install(TARGETS ${MOVEIT_LIB_NAME}
#   ARCHIVE DESTINATION lib
#   LIBRARY DESTINATION lib
#   RUNTIME DESTINATION bin)

ament_export_include_directories(include)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
