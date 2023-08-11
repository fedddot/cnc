add_library(serial_port_sender INTERFACE)
target_sources(serial_port_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/serial_port_sender.cpp)
target_link_libraries(
	serial_port_sender 
	INTERFACE 
	data_sender
)
target_include_directories(serial_port_sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)