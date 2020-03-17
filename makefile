SHELL=/bin/bash
all:
	make run
run:
	#colcon build --symlink-install
	colcon build --packages-select moveit_task_constructor
	bash refresh.sh
