include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(string STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/string.cpp)

add_subdirectory(tests)