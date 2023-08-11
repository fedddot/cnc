cmake_minimum_required(VERSION 3.10)

project(cnc)

set(DEV_DIR ${CMAKE_SOURCE_DIR}/..)
set(FRAMEWORK_DIR ${DEV_DIR}/framework)
set(GTEST_DIR ${DEV_DIR}/tools/googletest)

add_subdirectory(${FRAMEWORK_DIR} ${CMAKE_BINARY_DIR}/framework EXCLUDE_FROM_ALL)

set(IPC_C2S_FILE ${CMAKE_SOURCE_DIR}/c2s.dat)
set(IPC_S2C_FILE ${CMAKE_SOURCE_DIR}/s2c.dat)

add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(communication)
add_subdirectory(task)