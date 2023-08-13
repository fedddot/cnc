add_library(server_task_manager INTERFACE)
target_sources(server_task_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/server_task_manager.cpp)
target_link_libraries(
	server_task_manager 
	INTERFACE 
	ilistener
	itask
	task_factory
	idata
	string
	array
	object
	json_parser
	json_common
)
target_include_directories(server_task_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)



add_library(task_factory INTERFACE)
target_sources(task_factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/task_factory.cpp)
target_include_directories(task_factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	task_factory 
	INTERFACE 
	itask
	icreator
	
	idata
	string
	array
	object

	json_parser
	json_common
)






add_executable(server ${CMAKE_CURRENT_LIST_DIR}/src/server.cpp)
target_link_libraries(
	server
	PRIVATE
	# communication
	communication_manager
	server_data_sender
	server_data_receiver

	# tasks
	server_task_manager
	server_movement_task
	server_movement_task_creator
)
target_compile_definitions(
	server
	PRIVATE
	-DIPC_C2S_FILE=${IPC_C2S_FILE}
	-DIPC_S2C_FILE=${IPC_S2C_FILE}
)

add_subdirectory(communication)
add_subdirectory(tasks)