add_executable(
	task_factory_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_task_factory.cpp
)

target_link_libraries(
	task_factory_tests
	PRIVATE
	task_factory
	object
	
	gtest
	gmock
	-g
	-O0
)
