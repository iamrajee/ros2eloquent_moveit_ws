# created using 
ros2 pkg create --build-type ament_cmake_python --node-name cpp_node testpkg_cmake_python

# ======= then copied 5(3 folder and 2 files) ===========
testpkg_python/            => testpkg_cmake_python(changed folder name)\
resources/testpkg_python   => testpkg_cmake_python(changed file name)\
test/                      => #no change\
setup.py                   => testpkg_cmake_python(changed variable name)\
setup.cfg                  => testpkg_cmake_python(changed variable name)

# ======= Changes in package.xml ===========
added <build_type>ament_python</build_type> and many more

# ======= Changes in CMakelist (Not required!) ========
ament_python_install_package(${PROJECT_NAME})
install(PROGRAMS ${PROJECT_NAME}/py_node2.py DESTINATION lib/${PROJECT_NAME})
and many more ...