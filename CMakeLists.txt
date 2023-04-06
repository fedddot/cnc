cmake_minimum_required(VERSION 3.10)

project(cnc)

set(CMAKE_C_FLAGS "-std=c99 -pedantic-errors -Wall -Wextra -O0")
set(CMAKE_CXX_FLAGS "-std=c++17 -pedantic-errors -Wall -Wextra -O0")
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/cnc_server ${CMAKE_CURRENT_LIST_DIR}/cnc_server/build)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(
	message_parser
	SHARED
	${CMAKE_CURRENT_SOURCE_DIR}/src/message_parser.cpp
)
