cmake_minimum_required(VERSION 3.12)

project(cnc_server)

add_library(server_task_manager INTERFACE)
target_sources(server_task_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/server_task_manager.cpp)
target_link_libraries(server_task_manager INTERFACE ilistener isender idata iserver_task)
target_include_directories(server_task_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

set(FRAMEWORK_DIR ${CMAKE_SOURCE_DIR}/../../framework)
add_subdirectory(communication)
add_subdirectory(tasks)
add_subdirectory(${FRAMEWORK_DIR} ${CMAKE_BINARY_DIR}/framework)