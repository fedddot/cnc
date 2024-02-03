add_executable(
	engine_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_engine.cpp
)

target_link_libraries(
	engine_tests
	PRIVATE
	engine
	
	gtest
	gmock
	-g
	-O0
)
