add_library(server_data_sender INTERFACE)
target_sources(server_data_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/server_data_sender.cpp)
target_link_libraries(server_data_sender INTERFACE data_sender)
target_include_directories(server_data_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(server_message_listener INTERFACE)
target_sources(server_message_listener INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/server_message_listener.cpp)
target_link_libraries(
	server_message_listener 
	INTERFACE 
	ilistener
	idata
	idata_parser
	string
	array
	object
	json_parser
	json_common
)
target_include_directories(server_message_listener INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(server_movement_task INTERFACE)
target_sources(server_movement_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/server_movement_task.cpp)
target_include_directories(server_movement_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	server_movement_task 
	INTERFACE 
	movement_task
	idata
	object
	array
	string
	json_serializer
)

add_executable(server ${CMAKE_CURRENT_LIST_DIR}/src/server.cpp)
target_link_libraries(
	server
	PRIVATE
	server_movement_task
	server_data_sender
	data_receiver
	server_message_listener
)
target_compile_definitions(
	server
	PRIVATE
	-DIPC_C2S_FILE=${IPC_C2S_FILE}
	-DIPC_S2C_FILE=${IPC_S2C_FILE}
)