cmake_minimum_required(VERSION 3.10)

project(cnc)

set(DEV_DIR ${CMAKE_SOURCE_DIR}/..)
set(FRAMEWORK_DIR ${DEV_DIR}/framework)
set(GTEST_DIR ${DEV_DIR}/tools/googletest)

add_subdirectory(${FRAMEWORK_DIR} ${CMAKE_BINARY_DIR}/framework EXCLUDE_FROM_ALL)
add_subdirectory(${GTEST_DIR} ${CMAKE_BINARY_DIR}/gtest EXCLUDE_FROM_ALL)

add_subdirectory(client)