cmake_minimum_required(VERSION 3.28)
project(imgui)

add_library(${PROJECT_NAME} STATIC 
        imgui.cpp
        imgui_draw.cpp
        imgui_tables.cpp
        imgui_widgets.cpp
        imgui_impl_opengl3.cpp
        imgui_impl_sdl2.cpp
        imgui_demo.cpp
)

target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})