cmake_minimum_required(VERSION 3.10)

project(cnc_client)

add_library(client_movement_task INTERFACE)
target_sources(client_movement_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/client_movement_task.cpp)
target_include_directories(client_movement_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	client_movement_task 
	INTERFACE 
	itask
	movement_task
	
	idata
	string
	object
	
	isender
	
	json_serializer
)

add_library(client_sender INTERFACE)
target_sources(client_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/client_sender.cpp)
target_link_libraries(client_sender INTERFACE byte_sender idata iserializer json_serializer)
target_include_directories(client_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(client_receiver INTERFACE)
target_sources(client_receiver INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/client_receiver.cpp)
target_link_libraries(client_receiver INTERFACE byte_receiver idata iparser json_parser)
target_include_directories(client_receiver INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(client ${CMAKE_CURRENT_LIST_DIR}/src/client.cpp)
target_link_libraries(
	client
	PRIVATE
	client_sender
	client_receiver
	# client_movement_task
)

set(FRAMEWORK_DIR ${CMAKE_SOURCE_DIR}/../../framework)
add_subdirectory(${FRAMEWORK_DIR} ${CMAKE_BINARY_DIR}/framework)