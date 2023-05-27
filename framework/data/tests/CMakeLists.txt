add_executable(
	tests_data_exec
	${CMAKE_CURRENT_LIST_DIR}/src/tests_data.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_string.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_list.cpp
)

target_link_libraries(
	tests_data_exec
	PRIVATE
	ilistener_const_exception_ref
	exception_handler
	exception_handler_linux
	exception
	string
	gtest
	gmock
)
