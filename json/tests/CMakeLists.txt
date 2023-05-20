cmake_minimum_required(VERSION 3.10)

project(json_tests)

set(CMAKE_COLOR_MAKEFILE ON)

add_executable(
	${PROJECT_NAME}_exec
	${CMAKE_CURRENT_LIST_DIR}/src/json_tests.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_json_string.cpp
	# ${CMAKE_CURRENT_LIST_DIR}/src/ut_json_object.cpp
)

target_link_libraries(
	json_tests_exec
	PUBLIC
	ijson_value
	json_string
	# json_object
	# json_array
	json_utils
	iparsable
	string
	gtest
	gmock
)
