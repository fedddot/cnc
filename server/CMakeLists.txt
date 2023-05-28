cmake_minimum_required(VERSION 3.10)

project(cnc_server)

set(CMAKE_COLOR_MAKEFILE ON)

set(CNC_PATH ${CMAKE_SOURCE_DIR}/..)
set(DEV_PATH ${CNC_PATH}/..)
set(FRAMEWORK_PATH ${CNC_PATH}/framework)
set(GOOGLE_TESTS_PATH ${DEV_PATH}/tools/googletest)
set(TOOLCHAIN_PATH ${DEV_PATH}/tools/avr8-gnu-toolchain)

set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER usbASP)
set(AVR_UPLOADTOOL_PORT usb)
set(AVR_MCU avr3)
set(AVR_H_FUSE 0xd9)
set(AVR_L_FUSE 0xc3)
set(AVR_MCU_SPEED 16000000UL)

set(CMAKE_FIND_ROOT_PATH ${TOOLCHAIN_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_SYSTEM_INCLUDE_PATH ${TOOLCHAIN_PATH}/avr/include)
set(CMAKE_SYSTEM_LIBRARY_PATH ${TOOLCHAIN_PATH}/avr/lib)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/bin/avr-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/bin/avr-g++)

message(STATUS "Set CMAKE_FIND_ROOT_PATH to ${CMAKE_FIND_ROOT_PATH}")
message(STATUS "Set CMAKE_SYSTEM_INCLUDE_PATH to ${CMAKE_SYSTEM_INCLUDE_PATH}")
message(STATUS "Set CMAKE_SYSTEM_LIBRARY_PATH to ${CMAKE_SYSTEM_LIBRARY_PATH}")
message(STATUS "Set CMAKE_C_COMPILER to ${CMAKE_C_COMPILER}")
message(STATUS "Set CMAKE_CXX_COMPILER to ${CMAKE_CXX_COMPILER}")

if(CMAKE_BUILD_TYPE MATCHES Release)
   set(CMAKE_C_FLAGS_RELEASE "-O0")
   set(CMAKE_CXX_FLAGS_RELEASE "-O0")
endif(CMAKE_BUILD_TYPE MATCHES Release)

if(CMAKE_BUILD_TYPE MATCHES Debug)
   set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
   set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

add_definitions(
	-DF_CPU=${AVR_MCU_SPEED}
	-mmcu=${AVR_MCU}
	-Wall
	-Werror
	-pedantic
	-pedantic-errors
	-std=c++11
	-no-pie
)

add_subdirectory(${FRAMEWORK_PATH} ${CMAKE_SOURCE_DIR}/build/framework)

include_directories(
	${CMAKE_SOURCE_DIR}/include
	${FRAMEWORK_PATH}/include
	${FRAMEWORK_PATH}/except/include
	${FRAMEWORK_PATH}/data/include
	${FRAMEWORK_PATH}/json/include
	${FRAMEWORK_PATH}/memory/include
	${FRAMEWORK_PATH}/system/include
)

add_library(uart_message_manager OBJECT ${CMAKE_SOURCE_DIR}/src/uart_message_manager.cpp)
add_library(list_char OBJECT ${CMAKE_SOURCE_DIR}/src/list_char.cpp)
# add_library(features OBJECT ${CMAKE_SOURCE_DIR}/src/features.cpp)

add_executable(
	cnc_firmware
	${CMAKE_SOURCE_DIR}/src/cnc_firmware.cpp
)

target_link_libraries(
	cnc_firmware
	
	PRIVATE

	ilistener_const_exception_ref
	exception_handler
	exception_handler_avr
	exception
	string

	iparsable
	json_string
	json_object
	json_array
	json_utils
)

	

# target_link_libraries(
# 	cnc_firmware
# 	string
# 	json_object
# 	json_array
# 	ilistener_const_exception_ref
# 	ilistener_const_list_char_ref
# 	ilistener_char
# 	isender
# 	exception
# 	exception_handler
# 	exception_handler_avr
# 	message_parser
# 	message_manager
# 	uart_message_manager
# )