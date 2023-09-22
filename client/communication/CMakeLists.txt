add_library(client_uart INTERFACE)
target_sources(client_uart INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/client_uart.cpp)
target_include_directories(client_uart INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(client_uart INTERFACE ilistener uart)