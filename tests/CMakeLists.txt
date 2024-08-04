add_executable(
	cnc_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_stepper_motor.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_gpo_allocator.cpp
)

target_link_libraries(
	cnc_tests
	PRIVATE
	gpo_allocator
	custom_creator
	custom_task_executor
	json_data_serializer
	stepper_motor

	gtest
)