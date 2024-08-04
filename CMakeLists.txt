cmake_minimum_required(VERSION 3.12)

project(cnc C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
    mcu_factory
    task_executor
)

add_library(gpo_allocator INTERFACE)
target_include_directories(gpo_allocator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    gpo_allocator
    INTERFACE
    creator
    data_lib
    gpio
    mcu_factory
    task_executor
)

add_library(delay_allocator INTERFACE)
target_include_directories(delay_allocator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    delay_allocator
    INTERFACE
    creator
    data_lib
    mcu_factory
    task_executor
)



add_subdirectory(external)
add_subdirectory(util)
add_subdirectory(tests EXCLUDE_FROM_ALL)