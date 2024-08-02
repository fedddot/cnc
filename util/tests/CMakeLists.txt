add_executable(
	cnc_util_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/ut_task_object_generator.cpp
)

target_link_libraries(
	cnc_util_tests
	PRIVATE
    data_lib
    mcu_factory
    task_object_generator

	gtest
)