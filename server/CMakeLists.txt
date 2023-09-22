cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(cnc_server C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PICO_CXX_ENABLE_RTTI 1)

pico_sdk_init()

add_executable(server ${CMAKE_CURRENT_LIST_DIR}/src/server.cpp)
target_link_libraries(
	server
	PRIVATE
	server_uart
	package_manager
	package_descriptor
)

pico_add_extra_outputs(server)

set(FRAMEWORK_DIR ${CMAKE_SOURCE_DIR}/../../framework)
add_subdirectory(communication)
add_subdirectory(hardware)
add_subdirectory(tasks)
add_subdirectory(${FRAMEWORK_DIR} ${CMAKE_BINARY_DIR}/framework)