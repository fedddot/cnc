cmake_minimum_required(VERSION 3.13)

set(CNC_PATH ${CMAKE_SOURCE_DIR}/..)
set(FRAMEWORK_PATH ${CNC_PATH}/framework)
set(DEV_PATH ${CNC_PATH}/..)
set(GOOGLE_TESTS_PATH ${DEV_PATH}/tools/googletest)

project(cnc C CXX)

set(CMAKE_COLOR_MAKEFILE ON)

add_subdirectory(${FRAMEWORK_PATH} ${CMAKE_SOURCE_DIR}/build/framework)

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${FRAMEWORK_PATH}/include
	${FRAMEWORK_PATH}/json/include
	${FRAMEWORK_PATH}/system/include
)

add_executable(
	client
	${CMAKE_CURRENT_LIST_DIR}/src/client.cpp
)

target_link_libraries(
	client
	message_manager
	message_parser
	iparsable
	isender
	itask
	ilistener_const_vector_char_ref
	ilistener_char
	json_objects
	json_utils
)