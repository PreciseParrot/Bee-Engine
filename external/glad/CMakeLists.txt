cmake_minimum_required(VERSION 3.28)

target_sources(${PROJECT_NAME} PRIVATE
        src/gl.c
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)