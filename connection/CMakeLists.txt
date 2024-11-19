cmake_minimum_required(VERSION 3.28.3)

project(connections)

file(GLOB_RECURSE SOURCE
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_library(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME} PRIVATE
  "${PROJECT_SOURCE_DIR}/include"
)

target_link_libraries(${PROJECT_NAME}
  libiothub_client.a
  libiothub_client_mqtt_transport.a
  libumqtt.a
  libaziotsharedutil.a
  libparson.a
  libprov_auth_client.a
  libcurl.so
  libssl.so
  libcrypto.so
  libhsm_security_client.a
  libutpm.a
  libuuid.so
  libuhttp.a
  libmosquitto.so
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SYSROOT}/usr/local/lib)

file(GLOB_RECURSE HEADERS
  ${PROJECT_SOURCE_DIR}/include/*.h
)
install(FILES ${HEADERS}
    DESTINATION ${CMAKE_SYSROOT}/usr/local/include/${PROJECT_NAME})