cmake_minimum_required(VERSION 3.10)

project(cnc_client)

add_executable(client ${CMAKE_CURRENT_LIST_DIR}/src/client.cpp)
target_link_libraries(
	client
	PRIVATE
	client_uart
	package_descriptor
	package_manager
)

add_subdirectory(communication)
set(FRAMEWORK_DIR ${CMAKE_SOURCE_DIR}/../../framework)
add_subdirectory(${FRAMEWORK_DIR} ${CMAKE_BINARY_DIR}/framework)