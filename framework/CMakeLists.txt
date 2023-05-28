include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${CMAKE_CURRENT_SOURCE_DIR}/json/include
	${CMAKE_CURRENT_SOURCE_DIR}/system/include
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/json)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/system)

add_library(iparsable OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/src/iparsable.cpp)
add_library(itask OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/src/itask.cpp)
add_library(ilistener_const_exception_ref OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/src/ilistener_const_exception_ref.cpp)
add_library(ilistener_const_list_char_ref OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/src/ilistener_const_list_char_ref.cpp)
add_library(ilistener_char OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/src/ilistener_char.cpp)