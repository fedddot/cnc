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

add_library(data_sender INTERFACE)
target_sources(data_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/data_sender.cpp)
target_include_directories(data_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(data_receiver INTERFACE)
target_sources(data_receiver INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/data_receiver.cpp)
target_include_directories(data_receiver INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
	data_receiver
	INTERFACE
	ilistener
)

add_executable(client ${CMAKE_CURRENT_LIST_DIR}/src/client.cpp)
target_link_libraries(
	client
	PRIVATE
	movement_task
	data_sender
	data_receiver
)