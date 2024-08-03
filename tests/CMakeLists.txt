add_executable(
	cnc_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_persistent_task.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_stepper_motor.cpp
)

target_link_libraries(
	cnc_tests
	PRIVATE
	persistent_task
	data_lib
	custom_creator
	custom_task_executor
	json_data_serializer
    data_lib
    task_data_generator
	task_object_generator
	stepper_motor
	custom_task_executor
	json_data_serializer

	gtest
)