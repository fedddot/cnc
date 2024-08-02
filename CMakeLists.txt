cmake_minimum_required(VERSION 3.12)

project(cnc C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(cnc INTERFACE)
target_include_directories(cnc INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(cnc INTERFACE ipc_connection)

add_executable(cnc_app ${CMAKE_CURRENT_LIST_DIR}/src/cnc_app.cpp)
target_include_directories(cnc_app PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    cnc_app
    PRIVATE
    cnc
    linux_ipc_connection
)

add_subdirectory(external)
add_subdirectory(tests EXCLUDE_FROM_ALL)