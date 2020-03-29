from setuptools import setup

package_name = 'testpkg_action'

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
            'py_node = testpkg_action.py_node:main',
            'fibonacci_action_server = testpkg_action.fibonacci_action_server:main',
            'fibonacci_action_client = testpkg_action.fibonacci_action_client:main'
        ],
    },
)