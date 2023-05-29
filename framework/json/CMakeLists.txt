include_directories(
	${CMAKE_CURRENT_LIST_DIR}/include
)

add_library(
	json_objects 
	OBJECT
	${CMAKE_CURRENT_LIST_DIR}/src/json_string.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/json_object.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/json_array.cpp
)

add_library(
	json_headers
	INTERFACE
)
target_include_directories(
	json_headers
	INTERFACE
	${CMAKE_CURRENT_LIST_DIR}/include
)

add_library(
	json_utils 
	OBJECT 
	${CMAKE_CURRENT_LIST_DIR}/src/json_utils.cpp
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tests)
