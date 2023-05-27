include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(message_parser STATIC ${CMAKE_CURRENT_LIST_DIR}/src/message_parser.cpp)
target_link_libraries(
	message_parser
)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tests)