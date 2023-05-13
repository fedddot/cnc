cmake_minimum_required(VERSION 3.10)

set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER usbASP)
set(AVR_UPLOADTOOL_PORT usb)
set(AVR_MCU atmega328p)
set(AVR_H_FUSE 0xd9)
set(AVR_L_FUSE 0xc3)
set(AVR_MCU_SPEED 16000000UL)

project(cnc_server)

set(DEV_PATH ${CMAKE_SOURCE_DIR}/../..)
set(TOOLCHAIN_PATH ${DEV_PATH}/tools/avr8-gnu-toolchain)
set(CNC_PATH ${DEV_PATH}/cnc)

set(CMAKE_COLOR_MAKEFILE ON)
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
   set(CMAKE_C_FLAGS_RELEASE "-Os")
   set(CMAKE_CXX_FLAGS_RELEASE "-Os")
endif(CMAKE_BUILD_TYPE MATCHES Release)

if(CMAKE_BUILD_TYPE MATCHES Debug)
   set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
   set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

add_definitions("-DF_CPU=${AVR_MCU_SPEED}")
add_definitions("-mmcu=${AVR_MCU}")
add_definitions("-Wall")
add_definitions("-Werror")
add_definitions("-pedantic")
add_definitions("-pedantic-errors")
add_definitions("-std=c++11")

add_subdirectory(${CNC_PATH}/common ${CMAKE_BINARY_DIR}/common)
add_subdirectory(${CNC_PATH}/data ${CMAKE_BINARY_DIR}/data)
# add_subdirectory(${CNC_PATH}/json ${CMAKE_BINARY_DIR}/json)

include_directories(
	${CMAKE_CURRENT_LIST_DIR}/include
)

# add_library(
# 	mcu_resource_manager
# 	STATIC
# 	${CMAKE_CURRENT_LIST_DIR}/src/mcu_resource_manager.cpp
# )

# add_library(
# 	task_factory
# 	STATIC
# 	${CMAKE_CURRENT_LIST_DIR}/src/task_factory.cpp
# )

# add_library(
# 	report_status_task
# 	STATIC
# 	${CMAKE_CURRENT_LIST_DIR}/src/report_status_task.cpp
# )
# target_link_libraries(
# 	report_status_task
# 	itask
# )

# add_executable(
# 	cnc_firmware
# 	${CMAKE_CURRENT_LIST_DIR}/src/cnc_firmware.cpp
# )

# target_link_libraries(
# 	cnc_firmware
# 	mcu_resource_manager
# 	task_factory
	
# 	message_parser
# 	message
# 	ijson_value
# 	json_object
# 	json_string
# 	json_array
# 	json_utils
# 	ilistener_const_char_ref
# 	ilistener_const_message_ref
# 	iparsable
# 	report_status_task
# )