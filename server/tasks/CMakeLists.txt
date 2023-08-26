add_library(iserver_task INTERFACE)
target_include_directories(iserver_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(iserver_task INTERFACE idata)

add_library(server_movement_task INTERFACE)
target_sources(server_movement_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/server_movement_task.cpp)
target_include_directories(server_movement_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(server_movement_task INTERFACE movement_task iserver_task output_gpio)

add_library(server_movement_task_test STATIC)
target_link_libraries(server_movement_task_test PRIVATE server_movement_task)

add_library(server_movement_task_creator INTERFACE)
target_sources(server_movement_task_creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/server_movement_task_creator.cpp)
target_include_directories(server_movement_task_creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	server_movement_task_creator 
	INTERFACE 
	icreator
	
	iserver_task
	
	idata
	string
	array
	object

	server_movement_task
)

add_library(server_movement_task_creator_test STATIC)
target_link_libraries(server_movement_task_creator_test PRIVATE server_movement_task_creator)
