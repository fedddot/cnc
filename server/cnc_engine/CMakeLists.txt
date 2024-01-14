add_library(factory INTERFACE)
target_include_directories(factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(sender INTERFACE)
target_include_directories(sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(task INTERFACE)
target_include_directories(task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(engine INTERFACE)
target_include_directories(engine INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(tests)