
add_library(custom_task_executor INTERFACE)
target_include_directories(custom_task_executor INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(custom_task_executor INTERFACE task_executor)

add_library(task_object_generator INTERFACE)
target_include_directories(task_object_generator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    task_object_generator
    INTERFACE
    data_lib
    gpio
    mcu_factory
    task_data_generator
)

add_subdirectory(tests EXCLUDE_FROM_ALL)