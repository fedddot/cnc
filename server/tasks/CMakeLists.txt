add_library(inventory_task INTERFACE)
target_include_directories(inventory_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(inventory_task INTERFACE task inventory)

add_subdirectory(tests)