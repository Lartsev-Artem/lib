cmake_minimum_required(VERSION 3.6)

project(mylib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options("/Zc:__cplusplus")

include(${CMAKE_CURRENT_LIST_DIR}/cmake_module/cmake_files.cmake)  # функции cmake 

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/include/math    
    ${CMAKE_CURRENT_LIST_DIR}/include/io_api
    ${CMAKE_CURRENT_LIST_DIR}/include/math/optimization/evolution
    )

set(${PROJECT_NAME}_SRC 
     ${CMAKE_CURRENT_LIST_DIR}/src/io_api/header_rebuilder.cpp
     ${CMAKE_CURRENT_LIST_DIR}/src/math/FFT.cpp          
    )

get_all_files(${CMAKE_CURRENT_LIST_DIR}/src/math/optimization/evolution/*.cpp EVOLUTION_SRC)

#add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRC} )
add_library(${PROJECT_NAME} ${${PROJECT_NAME}_SRC} ${EVOLUTION_SRC})

add_subdirectory(tests)
