cmake_minimum_required(VERSION 3.13)

project(cnc)

set(CMAKE_COLOR_MAKEFILE ON)

set(DEV_PATH ${CMAKE_SOURCE_DIR}/..)

set(GOOGLE_TESTS_PATH ${DEV_PATH}/tools/googletest)

include_directories(
	${CMAKE_SOURCE_DIR}/framework/include
	${CMAKE_SOURCE_DIR}/framework/except/include
	${CMAKE_SOURCE_DIR}/framework/data/include
	${CMAKE_SOURCE_DIR}/framework/json/include
	${CMAKE_SOURCE_DIR}/framework/memory/include
	${CMAKE_SOURCE_DIR}/framework/system/include
	${GOOGLE_TESTS_PATH}/googletest/include
)
	
add_subdirectory(${GOOGLE_TESTS_PATH} ${CMAKE_BINARY_DIR}/gtest/build)
add_subdirectory(${CMAKE_SOURCE_DIR}/framework)