add_library(server_task INTERFACE)
target_sources(server_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/server_task.cpp)
target_include_directories(server_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(server_task INTERFACE itask object)



add_library(register_gpio_task INTERFACE)
target_sources(register_gpio_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/register_gpio_task.cpp)
target_include_directories(register_gpio_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(register_gpio_task INTERFACE server_task object gpio input_gpio output_gpio registry)

add_library(register_bridge_task INTERFACE)
target_sources(register_bridge_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/register_bridge_task.cpp)
target_include_directories(register_bridge_task INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(register_bridge_task INTERFACE server_task object bridge registry)



add_library(register_gpio_task_creator INTERFACE)
target_sources(register_gpio_task_creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/register_gpio_task_creator.cpp)
target_include_directories(register_gpio_task_creator INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(register_gpio_task_creator INTERFACE icreator server_task register_gpio_task)



add_library(server_task_manager INTERFACE)
target_sources(server_task_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/src/server_task_manager.cpp)
target_include_directories(server_task_manager INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(server_task_manager INTERFACE server_task factory object icreator package_manager register_gpio_task_creator json_parser json_serializer)