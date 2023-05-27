add_executable(
	tests_data_exec
	${CMAKE_CURRENT_LIST_DIR}/src/tests_data.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_string.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_shared_ptr.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_list.cpp
)

target_link_libraries(
	tests_data_exec
	PRIVATE
	string
	gtest
	gmock
	exception_handler
	exception_handler_linux
)
