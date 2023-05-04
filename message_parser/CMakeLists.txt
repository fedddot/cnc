cmake_minimum_required(VERSION 3.10)

project(message_parser)

set(CMAKE_COLOR_MAKEFILE ON)

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_CURRENT_LIST_DIR}/include
)

add_library(imessage_parser STATIC ${CMAKE_CURRENT_LIST_DIR}/src/imessage_parser.cpp)
add_library(imessage STATIC ${CMAKE_CURRENT_LIST_DIR}/src/imessage.cpp)
# add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tests)
