include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(exception STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/exception.cpp)
add_library(exception_handler STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/exception_handler.cpp)
add_library(exception_handler_linux STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/linux/exception_handler.cpp)
add_library(exception_handler_avr STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/avr/exception_handler.cpp)