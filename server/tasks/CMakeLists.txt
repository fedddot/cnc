add_library(server_task INTERFACE)
target_sources(server_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/server_task.cpp)
target_include_directories(server_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(server_task INTERFACE itask object)

add_library(server_task_test STATIC)
target_link_libraries(server_task_test PRIVATE server_task)


add_library(register_gpio_task INTERFACE)
target_sources(register_gpio_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/register_gpio_task.cpp)
target_include_directories(register_gpio_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(register_gpio_task INTERFACE server_task object gpio input_gpio output_gpio registry)

add_library(register_gpio_task_test STATIC)
target_link_libraries(register_gpio_task_test PRIVATE register_gpio_task)

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

add_library(server_task_manager INTERFACE)
target_sources(server_task_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/server_task_manager.cpp)
target_include_directories(server_task_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	server_task_manager 
	INTERFACE 
	ilistener 
	isender 
	idata 
	iserver_task 
	server_sender 
	server_receiver
	factory 
	icreator 
	itask
	server_movement_task_creator
	server_movement_task
	uart_handler
	hardware_rtc
)

add_library(server_movement_task_creator_test STATIC)
target_link_libraries(server_movement_task_creator_test PRIVATE server_movement_task_creator)
