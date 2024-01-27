cmake_minimum_required(VERSION 3.12)

project(cnc_server C CXX ASM)

if(NOT DEFINED DEV_DIR)
    set(DEV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
endif()

if(NOT DEFINED GTEST_DIR)
    set(GTEST_DIR ${DEV_DIR}/googletest)
endif()

add_subdirectory(${GTEST_DIR} ${CMAKE_BINARY_DIR}/gtest EXCLUDE_FROM_ALL)

add_subdirectory(basics)
add_subdirectory(cnc_engine)
add_subdirectory(data)
add_subdirectory(task_factory)
add_subdirectory(inventory)
add_subdirectory(tasks)

add_executable(server ${CMAKE_CURRENT_LIST_DIR}/src/server.cpp)
target_link_libraries(
    server
    PRIVATE
    creator
    
    engine
    task_factory
    create_inventory_item_task
)