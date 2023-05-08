cmake_minimum_required(VERSION 3.10)

project(cnc)

set(CMAKE_COLOR_MAKEFILE ON)

set(DEV_PATH ${CMAKE_SOURCE_DIR}/..)

set(GOOGLE_TESTS_PATH ${DEV_PATH}/googletest)

add_subdirectory(${GOOGLE_TESTS_PATH} ${CMAKE_SOURCE_DIR}/build/gtest/build)

include_directories(
	${GOOGLE_TESTS_PATH}/googletest/include
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/json/include
	${CMAKE_SOURCE_DIR}/message_parser/include
)

add_library(iparsable STATIC ${CMAKE_SOURCE_DIR}/src/iparsable.cpp)
add_library(ilistener_const_char_ref STATIC ${CMAKE_SOURCE_DIR}/src/ilistener_const_char_ref.cpp)
add_library(ilistener_const_message_ref STATIC ${CMAKE_SOURCE_DIR}/src/ilistener_const_message_ref.cpp)
add_library(itask STATIC ${CMAKE_SOURCE_DIR}/src/itask.cpp)

add_subdirectory(${CMAKE_SOURCE_DIR}/json)
add_subdirectory(${CMAKE_SOURCE_DIR}/message_parser)
add_subdirectory(${CMAKE_SOURCE_DIR}/onboard)