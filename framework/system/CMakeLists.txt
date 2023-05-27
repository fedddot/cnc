include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(isender OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/src/isender.cpp)
add_library(imessage_manager OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/src/imessage_manager.cpp)
