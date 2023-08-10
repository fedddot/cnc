add_library(movement_task INTERFACE)
target_sources(movement_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/movement_task.cpp)
target_include_directories(movement_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	movement_task 
	INTERFACE 
	itask
	idata
	string
	object
	array
	idata_serializer
	json_serializer
)

add_library(client_data_sender INTERFACE)
target_sources(client_data_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/client_data_sender.cpp)
target_link_libraries(client_data_sender INTERFACE data_sender)
target_include_directories(client_data_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)



add_executable(client ${CMAKE_CURRENT_LIST_DIR}/src/client.cpp)
target_link_libraries(
	client
	PRIVATE
	movement_task
	client_data_sender
	data_receiver
)