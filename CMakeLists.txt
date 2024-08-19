cmake_minimum_required(VERSION 3.6)

project(mylib)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/include/math
    ${CMAKE_CURRENT_LIST_DIR}/include/io_api
    )

set(${PROJECT_NAME}_SRC 
     ${CMAKE_CURRENT_LIST_DIR}/src/io_api/header_rebuilder.cpp
     
    )

#add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC}  ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SRC})

add_subdirectory(tests)
