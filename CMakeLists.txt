cmake_minimum_required(VERSION 3.12)

project(cnc C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(task_data_generator INTERFACE)
target_include_directories(task_data_generator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    task_data_generator
    INTERFACE
    data_lib
    gpio
    mcu_factory
)

add_library(stepper_motor INTERFACE)
target_include_directories(stepper_motor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    stepper_motor
    INTERFACE
    gpio
    json_data_parser
    json_data_serializer
    mcu_client
)

add_subdirectory(external)
add_subdirectory(tests EXCLUDE_FROM_ALL)