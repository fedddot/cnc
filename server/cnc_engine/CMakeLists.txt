add_library(report INTERFACE)
target_include_directories(report INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(report INTERFACE data)

add_library(engine INTERFACE)
target_sources(engine INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/engine.cpp)
target_include_directories(engine INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(engine INTERFACE data_lib task sender factory)

add_subdirectory(tests)