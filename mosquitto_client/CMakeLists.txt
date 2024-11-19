cmake_minimum_required(VERSION 3.28.3)

project(mosquitto_client)

file(GLOB_RECURSE SOURCE src/*.cpp)
add_library(${PROJECT_NAME} ${SOURCE})

target_include_directories(mosquitto_client PRIVATE
  "."
  "include"
)

target_link_libraries(${PROJECT_NAME} libmosquitto.so)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SYSROOT}/usr/local/lib)
install(FILES include/mosquitto_client.h
    DESTINATION ${CMAKE_SYSROOT}/usr/local/include/${PROJECT_NAME})