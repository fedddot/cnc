add_executable(
	inventory_tests
	${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
	${CMAKE_CURRENT_LIST_DIR}/src/ut_inventory.cpp
)

target_link_libraries(
	inventory_tests
	PRIVATE
	inventory
	
	gtest
	gmock
	-g
	-O0
)
