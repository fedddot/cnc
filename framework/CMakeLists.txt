include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/data/include
)

add_library(iparsable STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/iparsable.cpp)
add_library(itask STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/itask.cpp)