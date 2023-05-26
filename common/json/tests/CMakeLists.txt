cmake_minimum_required(VERSION 3.10)

project(json_tests)

set(CMAKE_COLOR_MAKEFILE ON)

include_directories(
	include
)

add_executable(
	json_tests_exec
	src/json_tests.cpp
	src/ut_json_string.cpp
	src/ut_json_object.cpp
	src/test_utils.cpp
)

target_link_libraries(
	json_tests_exec
	PUBLIC
	ijson_value
	json_string
	json_object
	json_array
	json_utils
	iparsable
	string
	gtest
	gmock
)
