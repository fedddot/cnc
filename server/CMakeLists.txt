cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(cnc_server C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PICO_CXX_ENABLE_RTTI 1)

pico_sdk_init()

add_library(server_task_manager INTERFACE)
target_sources(server_task_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/server_task_manager.cpp)
target_include_directories(server_task_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	server_task_manager 
	INTERFACE 
	ilistener 
	isender 
	idata 
	iserver_task 
	server_sender 
	factory 
	icreator 
	itask
	server_movement_task_creator
	server_movement_task
)

add_library(server_task_manager_test STATIC)
target_link_libraries(server_task_manager_test PRIVATE server_task_manager)

add_executable(server ${CMAKE_CURRENT_LIST_DIR}/src/server.cpp)
target_link_libraries(
	server
	PRIVATE
	server_task_manager
	server_sender
	pico_stdlib
)

set(FRAMEWORK_DIR ${CMAKE_SOURCE_DIR}/../../framework)
add_subdirectory(communication)
add_subdirectory(tasks)
add_subdirectory(${FRAMEWORK_DIR} ${CMAKE_BINARY_DIR}/framework)