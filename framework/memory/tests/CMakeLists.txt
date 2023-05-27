add_executable(
	tests_memory_exec
	${CMAKE_CURRENT_LIST_DIR}/src/tests_memory.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_shared_ptr.cpp
)

target_link_libraries(
	tests_memory_exec
	PRIVATE
	ilistener_const_exception_ref
	exception_handler
	exception_handler_linux
	exception
	string
	gtest
	gmock
)
