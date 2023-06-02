cmake_minimum_required(VERSION 3.15)

set(CNC_PATH ${CMAKE_SOURCE_DIR}/..)
set(DEV_PATH ${CNC_PATH}/..)
set(FRAMEWORK_PATH ${CNC_PATH}/framework)

include(pico_sdk_import.cmake)

project(cnc_server C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

set(CMAKE_COLOR_MAKEFILE ON)

add_subdirectory(${FRAMEWORK_PATH} ${CMAKE_SOURCE_DIR}/build/framework)

# after defining all INTERFACE targets in the Framework delete redundant include_directories
include_directories(
	${CMAKE_SOURCE_DIR}/include
	${FRAMEWORK_PATH}/include
	${FRAMEWORK_PATH}/json/include
	${FRAMEWORK_PATH}/system/include
)

add_library(uart_message_manager OBJECT ${CMAKE_SOURCE_DIR}/src/uart_message_manager.cpp)
target_link_libraries(
	uart_message_manager
	PRIVATE
	pico_stdlib_headers
	pico_stdio_uart_headers
)

add_executable(
	cnc_firmware
	${CMAKE_SOURCE_DIR}/src/cnc_firmware.cpp
)

target_link_libraries(
	cnc_firmware
	iparsable
	isender
	ilistener_char
	ilistener_const_vector_char_ref
	json_objects
	json_utils
	message_parser
	message_manager
	uart_message_manager
	
	pico_stdlib
)

pico_enable_stdio_usb(cnc_firmware 1)
pico_enable_stdio_uart(cnc_firmware 0)

pico_add_extra_outputs(cnc_firmware)