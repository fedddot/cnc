add_library(task_factory INTERFACE)
target_sources(task_factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/task_factory.cpp)
target_include_directories(task_factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(task_factory INTERFACE factory task data creator report)

add_subdirectory(tests)