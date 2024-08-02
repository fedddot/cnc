add_executable(
	cnc_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_stepper_motor.cpp
)

target_link_libraries(
	cnc_tests
	PRIVATE
    data_lib
    task_data_generator
	task_object_generator
	stepper_motor

	gtest
)