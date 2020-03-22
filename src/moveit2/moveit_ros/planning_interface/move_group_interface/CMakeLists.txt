set(MOVEIT_LIB_NAME moveit_move_group_interface)

add_library(${MOVEIT_LIB_NAME} SHARED src/move_group_interface.cpp)
set_target_properties(${MOVEIT_LIB_NAME} PROPERTIES VERSION "${${PROJECT_NAME}_VERSION}")

ament_target_dependencies(${MOVEIT_LIB_NAME}
    # moveit_planning_scene_interface
    rclcpp
    rclcpp_action
    moveit_core
    moveit_msgs
    moveit_ros_planning
    moveit_ros_move_group # <============ added new
)

target_link_libraries(${MOVEIT_LIB_NAME}
  moveit_common_planning_interface_objects
)
# add_library(${MOVEIT_LIB_NAME}_python SHARED src/wrap_python_move_group.cpp)
# target_link_libraries(${MOVEIT_LIB_NAME}_python
#   ${MOVEIT_LIB_NAME}
#   ${PYTHON_LIBRARIES}
#   ${Boost_LIBRARIES}
#   )
# add_dependencies(${MOVEIT_LIB_NAME}_python ${catkin_EXPORTED_TARGETS})
# set_target_properties(${MOVEIT_LIB_NAME}_python PROPERTIES VERSION "${${PROJECT_NAME}_VERSION}")
# set_target_properties(${MOVEIT_LIB_NAME}_python PROPERTIES OUTPUT_NAME _moveit_move_group_interface PREFIX "")
# set_target_properties(${MOVEIT_LIB_NAME}_python PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CATKIN_DEVEL_PREFIX}/${CATKIN_PACKAGE_PYTHON_DESTINATION}")

install(TARGETS ${MOVEIT_LIB_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

#TODO(anasarrak): re-enable when starting with python_move_group
# install(TARGETS ${MOVEIT_LIB_NAME}_python
#   ARCHIVE DESTINATION lib
#   LIBRARY DESTINATION lib)

install(DIRECTORY include/ DESTINATION include)

ament_export_libraries(${MOVEIT_LIB_NAME})                  #added new

add_executable(demo_move_group_interface src/demo.cpp)
ament_target_dependencies(demo_move_group_interface
  rclcpp
  Boost
  moveit_msgs
  moveit_ros_planning
)
target_link_libraries(demo_move_group_interface
  ${MOVEIT_LIB_NAME}
  moveit_common_planning_interface_objects
)
