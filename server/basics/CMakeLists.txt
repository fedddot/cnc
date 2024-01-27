add_library(builder INTERFACE)
target_include_directories(builder INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(creator INTERFACE)
target_include_directories(creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(factory INTERFACE)
target_include_directories(factory INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(sender INTERFACE)
target_include_directories(sender INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(task INTERFACE)
target_include_directories(task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)