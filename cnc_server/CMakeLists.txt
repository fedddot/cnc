set(CNC_PATH ${CMAKE_CURRENT_LIST_DIR}/..)

include_directories(
	${CNC_PATH}/include
	${CMAKE_CURRENT_LIST_DIR}/include
)

add_library(mcu_events SHARED ${CMAKE_CURRENT_LIST_DIR}/src/mcu_events.cpp)

add_executable(firmware ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp)
add_dependencies(firmware message_parser)
target_link_libraries(firmware PRIVATE mcu_events message_parser)
