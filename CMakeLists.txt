cmake_minimum_required(VERSION 3.10)

project(cnc)

set(CMAKE_COLOR_MAKEFILE ON)

set(DEV_PATH ${CMAKE_CURRENT_LIST_DIR}/..)
set(CNC_PATH ${DEV_PATH}/cnc)
set(JSON_PATH ${CNC_PATH}/json)

include_directories(
	$(CNC_PATH)/include
)

add_library(iparsable STATIC ${CNC_PATH}/src/iparsable.cpp)

add_subdirectory(${JSON_PATH})