add_library(client_movement_task INTERFACE)
target_sources(client_movement_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/client_movement_task.cpp)
target_include_directories(client_movement_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	client_movement_task 
	INTERFACE 
	movement_task
	idata
	object
	array
	string
	json_serializer
)

add_library(client_raw_data_sender INTERFACE)
target_sources(client_raw_data_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/client_raw_data_sender.cpp)
target_link_libraries(client_raw_data_sender INTERFACE raw_data_sender)
target_include_directories(client_raw_data_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)


add_library(raw_data_communication_manager INTERFACE)
target_sources(raw_data_communication_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/raw_data_communication_manager.cpp)
target_link_libraries(raw_data_communication_manager INTERFACE communication_manager client_raw_data_sender raw_data_receiver)
target_include_directories(raw_data_communication_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(raw_data_communication_manager_test STATIC)
target_link_libraries(raw_data_communication_manager_test PRIVATE raw_data_communication_manager)

add_executable(client ${CMAKE_CURRENT_LIST_DIR}/src/client.cpp)
target_link_libraries(
	client
	PRIVATE
	# client_movement_task
	
	raw_data_communication_manager
)
target_compile_definitions(
	client
	PRIVATE
	-DIPC_C2S_FILE=${IPC_C2S_FILE}
	-DIPC_S2C_FILE=${IPC_S2C_FILE}
)