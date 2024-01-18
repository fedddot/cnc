add_library(engine INTERFACE)
target_sources(engine INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/engine.cpp)
target_include_directories(engine INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(engine INTERFACE data)

add_subdirectory(data)
add_subdirectory(tests)