cmake_minimum_required(VERSION 3.10)

project(cnc)

set(DEV_DIR ${CMAKE_SOURCE_DIR}/..)
set(FRAMEWORK_DIR ${DEV_DIR}/framework)
set(GTEST_DIR ${DEV_DIR}/tools/googletest)

add_subdirectory(${FRAMEWORK_DIR} ${CMAKE_BINARY_DIR}/framework EXCLUDE_FROM_ALL)

add_library(movement_task INTERFACE)
target_sources(movement_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/movement_task.cpp)
target_include_directories(movement_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	movement_task 
	INTERFACE 
	itask
)

add_subdirectory(client)