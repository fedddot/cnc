add_library(inventory INTERFACE)
target_include_directories(inventory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(tests)