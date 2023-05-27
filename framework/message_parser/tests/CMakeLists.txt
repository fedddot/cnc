include_directories(
	${CMAKE_CURRENT_LIST_DIR}/include
)

add_library(
	test_message_listener
	OBJECT
	${CMAKE_CURRENT_LIST_DIR}/src/test_message_listener.cpp
)

add_executable(
	message_parser_tests_exec
	${CMAKE_CURRENT_LIST_DIR}/src/message_parser_tests.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_message_parser.cpp
)

target_link_libraries(
	message_parser_tests_exec
	PRIVATE
	ilistener_char
	ilistener_const_list_char_ref
	ilistener_const_exception_ref
	exception_handler
	exception_handler_linux
	exception
	string
	message_parser
	test_message_listener
	gtest
	gmock
)
