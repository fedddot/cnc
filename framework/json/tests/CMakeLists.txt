add_executable(
	json_tests_exec
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_json_parser.cpp
)

target_link_libraries(
	json_tests_exec
	PRIVATE
	
	json_parser
	json_common
	
	idata
	idata_parser
	
	array
	object
	string
	
	gtest
	gmock
)
