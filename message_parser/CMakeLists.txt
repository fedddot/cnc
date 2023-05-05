cmake_minimum_required(VERSION 3.10)

project(message_parser)

set(CMAKE_COLOR_MAKEFILE ON)

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_CURRENT_LIST_DIR}/include
)

add_library(message_parser STATIC ${CMAKE_CURRENT_LIST_DIR}/src/message_parser.cpp)
add_library(message STATIC ${CMAKE_CURRENT_LIST_DIR}/src/message.cpp)