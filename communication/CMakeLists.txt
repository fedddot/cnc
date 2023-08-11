add_library(data_receiver INTERFACE)
target_sources(data_receiver INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/data_receiver.cpp)
target_link_libraries(data_receiver INTERFACE ireceiver)
target_include_directories(data_receiver INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(data_sender INTERFACE)
target_sources(data_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/data_sender.cpp)
target_link_libraries(data_sender INTERFACE isender)
target_include_directories(data_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(communication_manager INTERFACE)
target_sources(communication_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/communication_manager.cpp)
target_link_libraries(communication_manager INTERFACE icommunication_manager)
target_include_directories(communication_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(communication_test STATIC)
target_link_libraries(
	communication_test 
	PRIVATE 
	data_receiver 
	data_sender 
	communication_manager
)