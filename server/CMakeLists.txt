cmake_minimum_required(VERSION 3.12)

project(cnc_server C CXX ASM)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED DEV_DIR)
    set(DEV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../..)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# add_subdirectory(${GTEST_DIR} ${CMAKE_BINARY_DIR}/gtest EXCLUDE_FROM_ALL)

add_subdirectory(engine)
add_subdirectory(task_factory)
add_subdirectory(inventory)
add_subdirectory(tasks)

add_executable(server ${CMAKE_CURRENT_LIST_DIR}/src/server.cpp)
target_link_libraries(
    server
    PRIVATE    
    engine
    task_factory
    inventory
    inventory_task
)