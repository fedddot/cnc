FetchContent_Declare(
    mcu_client
    GIT_REPOSITORY  https://github.com/fedddot/mcu_client.git
    GIT_TAG         main
)
FetchContent_GetProperties(mcu_client)
if(NOT mcu_client_POPULATED)
	FetchContent_Populate(mcu_client)
	add_subdirectory(${mcu_client_SOURCE_DIR} ${mcu_client_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()