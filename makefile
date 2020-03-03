SHELL=/bin/bash
all:
	make run
run:
	colcon build --symlink-install
	bash refresh.sh
