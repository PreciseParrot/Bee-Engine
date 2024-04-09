cmake_minimum_required(VERSION 3.27.6)

project(bee VERSION 0.0.1)

set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/bee/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})

add_library(${PROJECT_NAME} STATIC
    src/Audio.cpp
    src/Bee.cpp
    src/Collision/Collision.cpp
    src/Entity.cpp
    src/Log.cpp
    src/Graphics/HUDObject.cpp
    src/Graphics/Renderer.cpp
    src/Graphics/Sprite.cpp
    src/Input/Controller.cpp
    src/Input/Keyboard.cpp
    src/Input/Mouse.cpp
    src/World/World.cpp
    src/Math/Vector2f.cpp
    src/Math/Vector2i.cpp
)

option(BEE_VENDORED "Use vendored libraries" OFF)

if(BEE_VENDORED)
    set(SDL2IMAGE_VENDORED ON)
    set(SDL2MIXER_VENDORED ON)
    set(SDL2TTF_VENDORED ON)
    set(BUILD_SHARED_LIBS OFF)

    add_subdirectory(external/SDL2)
    add_subdirectory(external/SDL_image)
    add_subdirectory(external/SDL_mixer)
    add_subdirectory(external/SDL_ttf)
    add_subdirectory(external/json)
    add_subdirectory(external/tinyxml2)

    target_link_libraries(${PROJECT_NAME} SDL2::SDL2-static)
    target_link_libraries(${PROJECT_NAME} SDL2_mixer::SDL2_mixer-static)
    target_link_libraries(${PROJECT_NAME} SDL2_image::SDL2_image-static)
    target_link_libraries(${PROJECT_NAME} SDL2_ttf::SDL2_ttf-static)
    target_link_libraries(${PROJECT_NAME} tinyxml2::tinyxml2)
    target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
else()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_image REQUIRED)    
    find_package(SDL2_mixer REQUIRED)
    find_package(nlohmann_json REQUIRED)
    find_package(tinyxml2 REQUIRED)

    target_link_libraries(${PROJECT_NAME} SDL2::SDL2)
    target_link_libraries(${PROJECT_NAME} SDL2_mixer::SDL2_mixer)
    target_link_libraries(${PROJECT_NAME} SDL2_image::SDL2_image)
    target_link_libraries(${PROJECT_NAME} SDL2_ttf::SDL2_ttf)
    target_link_libraries(${PROJECT_NAME} tinyxml2::tinyxml2)
    target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(${PROJECT_NAME} 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include   
)

target_sources(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)