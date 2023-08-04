cmake_minimum_required(VERSION 3.10)

project(framework)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../tools/googletest ${CMAKE_BINARY_DIR}/gtest EXCLUDE_FROM_ALL)

add_subdirectory(data)
add_subdirectory(json)