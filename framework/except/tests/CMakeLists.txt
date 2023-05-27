add_executable(
	tests_except_exec
	${CMAKE_CURRENT_LIST_DIR}/src/tests_except.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_exception.cpp
)

target_link_libraries(
	tests_except_exec
	PRIVATE
	ilistener_const_exception_ref
	exception_handler
	exception_handler_linux
	exception
	string
	gtest
	gmock
)
