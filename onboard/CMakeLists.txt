cmake_minimum_required(VERSION 3.10)

project(onboard)

set(CMAKE_COLOR_MAKEFILE ON)

include_directories(
	${CMAKE_CURRENT_LIST_DIR}/include
)

add_library(
	mcu_resource_manager
	STATIC
	${CMAKE_CURRENT_LIST_DIR}/src/mcu_resource_manager.cpp
)

add_library(
	task_factory
	STATIC
	${CMAKE_CURRENT_LIST_DIR}/src/task_factory.cpp
)

add_library(
	report_status_task
	STATIC
	${CMAKE_CURRENT_LIST_DIR}/src/report_status_task.cpp
)
target_link_libraries(
	report_status_task
	itask
)

add_executable(
	cnc_firmware
	${CMAKE_CURRENT_LIST_DIR}/src/cnc_firmware.cpp
)

target_link_libraries(
	cnc_firmware
	mcu_resource_manager
	task_factory
	
	message_parser
	message
	ijson_value
	json_object
	json_string
	json_array
	json_utils
	ilistener_const_char_ref
	ilistener_const_message_ref
	iparsable
	report_status_task
)