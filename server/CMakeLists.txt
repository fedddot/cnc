cmake_minimum_required(VERSION 3.20)

set(CNC_PATH ${CMAKE_SOURCE_DIR}/..)
set(DEV_PATH ${CNC_PATH}/..)
set(FRAMEWORK_PATH ${CNC_PATH}/framework)
set(GOOGLE_TESTS_PATH ${DEV_PATH}/tools/googletest)
set(PICO_SDK_PATH ${DEV_PATH}/pico-sdk)

set(PICO_PLATFORM rp2040)

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(cnc_server C CXX ASM)

pico_sdk_init()

set(CMAKE_COLOR_MAKEFILE ON)

add_subdirectory(${FRAMEWORK_PATH} ${CMAKE_SOURCE_DIR}/build/framework)

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${FRAMEWORK_PATH}/include
	${FRAMEWORK_PATH}/json/include
	${FRAMEWORK_PATH}/system/include

	${PICO_SDK_PATH}/src/boards/include/boards
	${PICO_SDK_PATH}/src/common/pico_stdlib/include
	${PICO_SDK_PATH}/src/common/pico_time/include
	${PICO_SDK_PATH}/src/rp2_common/pico_stdio/include
	${PICO_SDK_PATH}/src/rp2_common/hardware_timer/include
	${PICO_SDK_PATH}/src/rp2040/hardware_structs/include
)

add_library(uart_message_manager OBJECT ${CMAKE_SOURCE_DIR}/src/uart_message_manager.cpp)

add_executable(
	cnc_firmware
	${CMAKE_SOURCE_DIR}/src/cnc_firmware.cpp
)

target_link_libraries(
	cnc_firmware
	iparsable
	json_object
	json_array
	json_string
	uart_message_manager
	ilistener_char
	isender
	message_parser
	message_manager
	pico_stdlib
)

pico_add_extra_outputs(cnc_firmware)