add_library(icnc_task INTERFACE)
target_include_directories(icnc_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(icnc_task INTERFACE itask)

add_library(movement_task INTERFACE)
target_sources(movement_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/movement_task.cpp)
target_include_directories(movement_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(movement_task INTERFACE icnc_task)
