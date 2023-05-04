cmake_minimum_required(VERSION 3.10)

project(json_tests)

set(CMAKE_COLOR_MAKEFILE ON)

set(GOOGLE_TESTS_PATH ${DEV_PATH}/googletest)

add_subdirectory(${GOOGLE_TESTS_PATH} ${CMAKE_CURRENT_LIST_DIR}/build/googletest_build)

include_directories(
	${GOOGLE_TESTS_PATH}/googletest/include
)

add_executable(
	${PROJECT_NAME}_exec
	json_tests.cpp
	ut_json_string.cpp
	ut_json_object.cpp
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
	gtest
	gmock
)
