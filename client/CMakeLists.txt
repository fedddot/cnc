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

add_library(client_data_sender INTERFACE)
target_sources(client_data_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/client_data_sender.cpp)
target_link_libraries(client_data_sender INTERFACE raw_data_sender)
target_include_directories(client_data_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)




add_executable(client ${CMAKE_CURRENT_LIST_DIR}/src/client.cpp)
target_link_libraries(
	client
	PRIVATE	
	communication_manager
)
target_compile_definitions(
	client
	PRIVATE
	-DIPC_C2S_FILE=${IPC_C2S_FILE}
	-DIPC_S2C_FILE=${IPC_S2C_FILE}
)