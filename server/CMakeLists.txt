cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(cnc_server C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_CXX_ENABLE_EXCEPTIONS 1)
set(PICO_CXX_ENABLE_RTTI 1)

set(DEV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)

set(OPENOCD_DIR ${DEV_DIR}/openocd)
set(DBG_INTERFACE cmsis-dap.cfg)
set(DBG_TARGET rp2040.cfg)

set(CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/config)
set(VSCODE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.vscode)

pico_sdk_init()

add_custom_target(
	start_openocd
	COMMENT "Starting openocd (interface = ${DBG_INTERFACE}, target = ${DBG_TARGET})"
	COMMAND sudo src/openocd -f interface/${DBG_INTERFACE} -c "adapter speed 5000" -f target/${DBG_TARGET} -s tcl
	WORKING_DIRECTORY ${OPENOCD_DIR}
)

add_custom_target(
	copy_config_files
	COMMENT "Copying config files from ${CONFIG_DIR} to ${VSCODE_DIR}"
	COMMAND mkdir -p ${VSCODE_DIR} &&  cp launch.json ${VSCODE_DIR} && cp settings.json ${VSCODE_DIR}
	WORKING_DIRECTORY ${CONFIG_DIR}
)

add_executable(server ${CMAKE_CURRENT_LIST_DIR}/src/server.cpp)
target_link_libraries(
	server
	PRIVATE
	uart
	server_uart
	package_manager
	package_descriptor
	server_task_manager
	output_gpio
)
target_compile_options(server PRIVATE -g)

pico_add_extra_outputs(server)

add_subdirectory(hardware)
add_subdirectory(tasks)
set(FRAMEWORK_DIR ${CMAKE_SOURCE_DIR}/../../framework)
add_subdirectory(${FRAMEWORK_DIR} ${CMAKE_BINARY_DIR}/framework)