add_library(task_factory INTERFACE)
target_include_directories(task_factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(task_factory INTERFACE factory task data)

add_subdirectory(tests EXCLUDE_FROM_ALL)