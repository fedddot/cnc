cmake_minimum_required(VERSION 3.12)

project(cnc C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(task_data_generator INTERFACE)
target_include_directories(task_data_generator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(task_data_generator INTERFACE creator)

add_subdirectory(external)
add_subdirectory(tests EXCLUDE_FROM_ALL)