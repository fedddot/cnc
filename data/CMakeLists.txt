include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(string STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/string.cpp)
# add_library(ilistener_const_char_ref STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/ilistener_const_char_ref.cpp)
# add_library(ilistener_const_message_ref STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/ilistener_const_message_ref.cpp)
# add_library(itask STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/itask.cpp)

add_subdirectory(tests)