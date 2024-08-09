cmake_minimum_required(VERSION 3.12)

project(cnc C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(task_executor INTERFACE)
target_include_directories(task_executor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(stepper_motor_types INTERFACE)
target_include_directories(stepper_motor_types INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(stepper_motor_types INTERFACE gpio)

add_library(server_object_allocator INTERFACE)
target_include_directories(server_object_allocator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(persistent_task_allocator INTERFACE)
target_include_directories(persistent_task_allocator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(persistent_task_allocator INTERFACE server_object_allocator)

add_library(custom_persistent_task_allocator INTERFACE)
target_include_directories(custom_persistent_task_allocator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(custom_persistent_task_allocator INTERFACE persistent_task_allocator server_object_allocator mcu_factory stepper_motor_types)

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

add_library(state_allocator INTERFACE)
target_include_directories(state_allocator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    state_allocator
    INTERFACE
    array
    custom_persistent_task_allocator
    gpo_allocator
    integer
    mcu_factory
    object
    persistent_task_allocator
    server_object_allocator
    stepper_motor_types
)



add_library(state_executor INTERFACE)
target_include_directories(state_executor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    state_executor
    INTERFACE
    creator
    data_lib
    delay_allocator
    gpo_allocator
    mcu_factory
    stepper_motor_types
    task_executor
)

add_library(stepper_motor INTERFACE)
target_include_directories(stepper_motor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    stepper_motor
    INTERFACE
    data_lib
    creator
    gpo_allocator
    mcu_factory
    state_allocator
    stepper_motor_types
    task_executor
)







add_subdirectory(external)
add_subdirectory(util)
add_subdirectory(tests EXCLUDE_FROM_ALL)