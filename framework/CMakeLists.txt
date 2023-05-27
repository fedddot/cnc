include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/data/include
	${CMAKE_CURRENT_SOURCE_DIR}/memory/include
	${CMAKE_CURRENT_SOURCE_DIR}/except/include
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/data)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/except)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/json)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/memory)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/message_parser)

add_library(iparsable STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/iparsable.cpp)
add_library(itask STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/itask.cpp)
add_library(ilistener_const_exception_ref STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/ilistener_const_exception_ref.cpp)