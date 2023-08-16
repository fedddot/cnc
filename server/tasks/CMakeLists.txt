add_library(server_movement_task INTERFACE)
target_sources(server_movement_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/server_movement_task.cpp)
target_include_directories(server_movement_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(server_movement_task INTERFACE movement_task isender server_sender)

add_library(server_movement_task_test STATIC)
target_link_libraries(server_movement_task_test PRIVATE server_movement_task)

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

add_library(server_movement_task_creator_test STATIC)
target_link_libraries(server_movement_task_creator_test PRIVATE server_movement_task_creator)
