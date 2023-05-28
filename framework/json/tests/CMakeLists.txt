include_directories(
	${CMAKE_CURRENT_LIST_DIR}/include
)

add_executable(
	json_tests_exec
	src/json_tests.cpp
	src/ut_json_string.cpp
	src/ut_json_object.cpp
)

target_link_libraries(
	json_tests_exec
	PRIVATE
	json_string
	json_object
	json_array
	json_utils
	iparsable
	gtest
	gmock
)
