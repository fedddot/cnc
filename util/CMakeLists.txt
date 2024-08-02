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