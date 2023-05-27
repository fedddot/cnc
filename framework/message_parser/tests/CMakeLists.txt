cmake_minimum_required(VERSION 3.10)

project(message_parser_tests)

set(CMAKE_COLOR_MAKEFILE ON)

include_directories(
	${CMAKE_CURRENT_LIST_DIR}/include
)

add_library(
	test_message_listener
	STATIC
	${CMAKE_CURRENT_LIST_DIR}/src/test_message_listener.cpp
)

# target_link_libraries(
# 	test_message_listener
# 	PUBLIC
# 	ilistener_const_message_ref
# )

# add_executable(
# 	${PROJECT_NAME}_exec
# 	${CMAKE_CURRENT_LIST_DIR}/src/message_parser_tests.cpp
# 	${CMAKE_CURRENT_LIST_DIR}/src/ut_message_parser.cpp
# )

# target_link_libraries(
# 	${PROJECT_NAME}_exec
# 	PUBLIC
# 	message_parser
# 	message
# 	test_message_listener
# 	gtest
# 	gmock
# )
