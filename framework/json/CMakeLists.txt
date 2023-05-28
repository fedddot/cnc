include_directories(
	${CMAKE_CURRENT_LIST_DIR}/include
)

add_library(json_string OBJECT ${CMAKE_CURRENT_LIST_DIR}/src/json_string.cpp)
add_library(json_object OBJECT ${CMAKE_CURRENT_LIST_DIR}/src/json_object.cpp)
add_library(json_array OBJECT ${CMAKE_CURRENT_LIST_DIR}/src/json_array.cpp)
add_library(json_utils OBJECT ${CMAKE_CURRENT_LIST_DIR}/src/json_utils.cpp)

# add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tests)
