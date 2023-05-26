cmake_minimum_required(VERSION 3.10)

project(cnc)

set(CMAKE_COLOR_MAKEFILE ON)

set(DEV_PATH ${CMAKE_SOURCE_DIR}/..)

set(GOOGLE_TESTS_PATH ${DEV_PATH}/tools/googletest)

include_directories(
	${CMAKE_SOURCE_DIR}/common/include
	${CMAKE_SOURCE_DIR}/data/include
	${CMAKE_SOURCE_DIR}/json/include
	${GOOGLE_TESTS_PATH}/googletest/include
)
	
add_subdirectory(${GOOGLE_TESTS_PATH} ${CMAKE_BINARY_DIR}/gtest/build)
add_subdirectory(${CMAKE_SOURCE_DIR}/common)
add_subdirectory(${CMAKE_SOURCE_DIR}/data)
add_subdirectory(${CMAKE_SOURCE_DIR}/json)
# add_subdirectory(${CMAKE_SOURCE_DIR}/message_parser)
# add_subdirectory(${CMAKE_SOURCE_DIR}/server)
