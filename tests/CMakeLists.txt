cmake_minimum_required(VERSION 3.6)

 #project(tests)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/build)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake_module/cmake_files.cmake)  # функции cmake 

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/../include    
    )

get_all_files(${CMAKE_CURRENT_LIST_DIR}/../src/math/optimization/evolution/*.cpp EVOLUTION_SRC)

set(${PROJECT_NAME}_LIB_SRC 
    ${CMAKE_CURRENT_LIST_DIR}/../src/io_api/header_rebuilder.cpp 
    ${EVOLUTION_SRC}
    )


get_all_files(${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${PROJECT_NAME}_EXE_SRC)

foreach(sub_prog ${${PROJECT_NAME}_EXE_SRC})
        get_filename_component(name ${sub_prog}  NAME_WLE) #имя файла без пути и без разширения
        add_executable(${name} ${${PROJECT_NAME}_LIB_SRC} ${sub_prog})                
endforeach()

target_link_libraries(mylib)