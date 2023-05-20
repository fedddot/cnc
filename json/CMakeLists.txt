include_directories(
	${CMAKE_CURRENT_LIST_DIR}/include
	${CMAKE_SOURCE_DIR}/common/include
	${CMAKE_SOURCE_DIR}/data/include
)

add_library(ijson_value STATIC ${CMAKE_CURRENT_LIST_DIR}/src/ijson_value.cpp)
add_library(json_string STATIC ${CMAKE_CURRENT_LIST_DIR}/src/json_string.cpp)
# add_library(json_object STATIC ${CMAKE_CURRENT_LIST_DIR}/src/json_object.cpp)
# add_library(json_array STATIC ${CMAKE_CURRENT_LIST_DIR}/src/json_array.cpp)
add_library(json_utils STATIC ${CMAKE_CURRENT_LIST_DIR}/src/json_utils.cpp)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tests)
