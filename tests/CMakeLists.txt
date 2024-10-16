add_library(cnc_fixture INTERFACE)
target_include_directories(cnc_fixture INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	cnc_fixture
	INTERFACE
    data_lib
    gpo
    ipc_connection
    json_data_serializer
    linux_ipc_connection
    mcu_client
    mcu_server
    remote_stepper_motor
	gtest
)

add_executable(
	cnc_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_remote_stepper_motor.cpp
)

target_link_libraries(
	cnc_tests
	PRIVATE
	custom_task_executor
	remote_stepper_motor
	cnc_fixture
	gtest
)