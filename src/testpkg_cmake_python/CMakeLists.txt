cmake_minimum_required(VERSION 3.5)
project(testpkg_cmake_python)

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
# =========================================================================================================== #



# catkin_install_python()


# find dependencies
find_package(ament_cmake REQUIRED)

### # new
### find_package(ament_cmake_python REQUIRED)
### find_package(rclcpp REQUIRED)
### find_package(rclpy REQUIRED)
### # Install the python module for this package
### ament_python_install_package(${PROJECT_NAME})
### include_directories(include ${rclcpp_INCLUDE_DIRS})
### link_directories(${rclcpp_LIBRARY_DIRS})
### # Install python scripts
### # install(PROGRAMS scripts/my_executable DESTINATION bin)
### # install(PROGRAMS ${PROJECT_NAME}/py_node2.py DESTINATION bin)
### # install(PROGRAMS ${PROJECT_NAME}/py_node2.py DESTINATION lib/${PROJECT_NAME})                    #<=== runtime error ???
### # install(TARGETS ${PROJECT_NAME} DESTINATION "${PYTHON_INSTALL_DIR}/${PROJECT_NAME}")             #<=== not a target
### # install(TARGETS ${PROJECT_NAME}/py_node4.py DESTINATION "${PYTHON_INSTALL_DIR}/${PROJECT_NAME}") #<=== not a target
### # install(PROGRAMS ${PROJECT_NAME}/py_node4.py DESTINATION "${PYTHON_INSTALL_DIR}/${PROJECT_NAME}")  #<=== does nothing
### # install(PROGRAMS ${PROJECT_NAME}/py_node5.py DESTINATION bin) 
### # add_executable(py_node5.py ${PROJECT_NAME}/py_node5.py)
### # # ament_target_dependencies(py_node5.py rclpy)
### # install(TARGETS py_node5.py DESTINATION "${PYTHON_INSTALL_DIR}/${PROJECT_NAME}")
### # install(PROGRAMS ${PROJECT_NAME}/py_node5.py DESTINATION bin) #no effect
### # install(PROGRAMS ${PROJECT_NAME}/py_node5.py DESTINATION lib/${PROJECT_NAME})

add_executable(cpp_node src/cpp_node.cpp)
target_include_directories(cpp_node PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:include>)
install(TARGETS cpp_node EXPORT export_${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})

### # Export package dependencies
### ament_export_dependencies(ament_cmake)
### ament_export_dependencies(ament_cmake_python)
### ament_export_dependencies(rclcpp)
### ament_export_dependencies(rclpy)
### ament_export_include_directories(include ${rclcpp_INCLUDE_DIRS})
### ament_export_libraries(${PROJECT_NAME} ${rclcpp_LIBRARIES})



# =========================================================================================================== #
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
