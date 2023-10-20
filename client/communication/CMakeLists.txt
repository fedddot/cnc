add_library(client_uart INTERFACE)
target_sources(client_uart INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/client_uart.cpp)
target_include_directories(client_uart INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(client_uart INTERFACE ilistener uart)

add_library(task_result_listener INTERFACE)
target_sources(task_result_listener INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/task_result_listener.cpp)
target_include_directories(task_result_listener INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(task_result_listener INTERFACE ilistener object json_serializer json_parser)