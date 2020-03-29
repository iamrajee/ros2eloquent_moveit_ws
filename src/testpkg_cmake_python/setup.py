from setuptools import setup

package_name = 'testpkg_cmake_python'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='rajendra',
    maintainer_email='singh.raj1997@gmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'py_node = testpkg_cmake_python.py_node:main',
            'py_node2 = testpkg_cmake_python.py_node2:main',
            'py_node3.py = testpkg_cmake_python.py_node3:main',
            'py_node5.py = testpkg_cmake_python.py_node5:main'
        ],
    },
)
