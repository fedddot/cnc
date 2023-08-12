add_library(server_data_sender INTERFACE)
target_sources(server_data_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/server_data_sender.cpp)
target_link_libraries(server_data_sender INTERFACE data_sender)
target_include_directories(server_data_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(server_data_receiver INTERFACE)
target_sources(server_data_receiver INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/server_data_receiver.cpp)
target_link_libraries(server_data_receiver INTERFACE data_receiver)
target_include_directories(server_data_receiver INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

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

add_library(server_movement_task_creator INTERFACE)
target_sources(server_movement_task_creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/server_movement_task_creator.cpp)
target_include_directories(server_movement_task_creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	server_movement_task_creator 
	INTERFACE 
	itask
	icreator
	
	idata
	string
	array
	object

	isender

	server_movement_task
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