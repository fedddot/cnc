add_library(json_parser INTERFACE)
target_sources(json_parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/json_parser.cpp)
target_include_directories(
	json_parser 
	INTERFACE 
	${CMAKE_CURRENT_LIST_DIR}/include
	${CMAKE_SOURCE_DIR}/data/include
)

add_library(json_common INTERFACE)
target_sources(json_common INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/json_common.cpp)
target_include_directories(
	json_common 
	INTERFACE 
	${CMAKE_CURRENT_LIST_DIR}/include
)

# delete it
add_library(json_test STATIC)
target_link_libraries(json_test PRIVATE json_parser json_common)