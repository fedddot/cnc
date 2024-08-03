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
)

add_library(task_executor INTERFACE)
target_include_directories(task_executor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(persistent_task INTERFACE)
target_include_directories(persistent_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    persistent_task
    INTERFACE
    task_executor
    creator
)

add_library(stepper_motor_state INTERFACE)
target_include_directories(stepper_motor_state INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(stepper_motor_state INTERFACE gpio)

add_library(stepper_motor INTERFACE)
target_include_directories(stepper_motor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    stepper_motor
    INTERFACE
    gpio
    data_lib
    task_data_generator
    task_executor
)

add_subdirectory(external)
add_subdirectory(util)
add_subdirectory(tests EXCLUDE_FROM_ALL)