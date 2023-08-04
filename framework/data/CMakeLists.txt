add_library(idata INTERFACE)
target_sources(idata INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/idata.cpp)
target_include_directories(idata INTERFACE INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(idata_parser INTERFACE)
target_sources(idata INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/idata_parser.cpp)
target_include_directories(idata_parser INTERFACE INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(composite INTERFACE)
target_sources(composite INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/composite.cpp)
target_include_directories(composite INTERFACE INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(object INTERFACE)
target_sources(object INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/object.cpp)
target_include_directories(object INTERFACE INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(array INTERFACE)
target_sources(array INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/array.cpp)
target_include_directories(array INTERFACE INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

add_library(string INTERFACE)
target_sources(string INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/string.cpp)
target_include_directories(string INTERFACE INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)