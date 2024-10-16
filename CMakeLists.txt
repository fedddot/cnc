cmake_minimum_required(VERSION 3.12)

project(cnc C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(task_executor INTERFACE)
target_include_directories(task_executor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(custom_task_executor INTERFACE)
target_include_directories(custom_task_executor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(custom_task_executor INTERFACE task_executor)

add_library(remote_stepper_motor INTERFACE)
target_include_directories(remote_stepper_motor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    remote_stepper_motor
    INTERFACE
    data_lib
    gpo
    task_executor
    stepper_motor_tasks_factory
)

add_subdirectory(external)
add_subdirectory(tests EXCLUDE_FROM_ALL)