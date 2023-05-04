cmake_minimum_required(VERSION 3.10)

project(cnc)

set(CMAKE_COLOR_MAKEFILE ON)

set(DEV_PATH ${CMAKE_SOURCE_DIR}/..)

include_directories(
	$(CMAKE_SOURCE_DIR)/include
)

add_library(iparsable STATIC ${CMAKE_SOURCE_DIR}/src/iparsable.cpp)

add_subdirectory(${CMAKE_SOURCE_DIR}/json)