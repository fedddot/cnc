add_library(gpio INTERFACE)
target_sources(gpio INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/gpio.cpp)
target_include_directories(gpio INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(gpio INTERFACE pico_stdlib)

add_library(input_gpio INTERFACE)
target_sources(input_gpio INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/input_gpio.cpp)
target_include_directories(input_gpio INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(input_gpio INTERFACE gpio pico_stdlib)

add_library(output_gpio INTERFACE)
target_sources(output_gpio INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/output_gpio.cpp)
target_include_directories(output_gpio INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(output_gpio INTERFACE gpio pico_stdlib)

add_library(server_uart INTERFACE)
target_sources(server_uart INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/server_uart.cpp)
target_include_directories(server_uart INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(server_uart INTERFACE ilistener uart pico_stdlib)

add_library(server_uart_test STATIC)
target_link_libraries(server_uart_test PRIVATE server_uart)