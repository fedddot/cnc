include_directories(
	${CMAKE_CURRENT_LIST_DIR}/include
)

add_library(
	test_message_listener
	OBJECT
	${CMAKE_CURRENT_LIST_DIR}/src/test_message_listener.cpp
)

add_executable(
	system_tests_exec
	${CMAKE_CURRENT_LIST_DIR}/src/system_tests.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_message_parser.cpp
)

target_link_libraries(
	system_tests_exec
	PRIVATE
	ilistener_char
	ilistener_const_vector_char_ref
	json_utils
	message_parser
	test_message_listener
	gtest
	gmock
)
