add_library(task_object_generator INTERFACE)
target_include_directories(task_object_generator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(
    task_object_generator
    INTERFACE
    task_data_generator
    data_lib
    gpio
    mcu_factory
)

add_subdirectory(tests EXCLUDE_FROM_ALL)