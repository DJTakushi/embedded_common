cmake_minimum_required(VERSION 3.28.3)

project(data_module_base)

file(GLOB_RECURSE SOURCE
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_library(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME} PRIVATE
  "include"
  "../common/include"
  "../connection/include"
  "../attributes/include"
  "../parser/include"
  )


target_link_libraries(${PROJECT_NAME}
  connections
  attributes
)