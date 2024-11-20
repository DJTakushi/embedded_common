cmake_minimum_required(VERSION 3.28.3)

set(EMBEDDED_COMMON_PROJECT_NAME        "embedded common")
set(EMBEDDED_COMMON_PROJECT_DESCRIPTION "Embedded Common")
set(EMBEDDED_COMMON_AUTHOR_DOMAIN "https://github.com/DJTakushi/embedded_common")
set(EMBEDDED_COMMON_VERSION_MAJOR "0")
set(EMBEDDED_COMMON_VERSION_MINOR "0")
set(EMBEDDED_COMMON_VERSION_PATCH "0")

project(${EMBEDDED_COMMON_PROJECT_NAME})

add_subdirectory(common)
add_subdirectory(connection)
add_subdirectory(mosquitto_client)