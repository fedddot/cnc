cmake_minimum_required(VERSION 3.12)

project(cnc_server C CXX ASM)

# UTs framework
set(DEV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(GTEST_DIR ${DEV_DIR}/tools/googletest)

add_subdirectory(${GTEST_DIR} ${CMAKE_BINARY_DIR}/gtest EXCLUDE_FROM_ALL)

add_subdirectory(basics)
add_subdirectory(cnc_engine)
add_subdirectory(data)
add_subdirectory(task_factory)
add_subdirectory(inventory)
add_subdirectory(tasks)