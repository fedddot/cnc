add_executable(
	tasks_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_tasks.cpp
)

target_link_libraries(
	tasks_tests
	PRIVATE
	inventory_task
	create_inventory_item_task
	delete_inventory_item_task

	data_lib
	
	gtest
	gmock
	-g
	-O0
)
