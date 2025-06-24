cmake_minimum_required(VERSION 3.28)
project(GLAD C)

add_library(${PROJECT_NAME} STATIC
        src/gl.c
        #src/gles2.c
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
