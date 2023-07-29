add_library(json_parser INTERFACE)
target_sources(json_parser INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/json_parser.cpp)
target_include_directories(
	json_parser 
	INTERFACE 
	${CMAKE_CURRENT_LIST_DIR}/include
	${CMAKE_CURRENT_LIST_DIR}/../include
)