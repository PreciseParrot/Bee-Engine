cmake_minimum_required(VERSION 3.27.6)

project(bee VERSION 0.0.1)

set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/bee/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})

add_library(${PROJECT_NAME} STATIC
    src/Audio.cpp
    src/Bee.cpp
    src/Collision/Collision.cpp
    src/Entity.cpp
    src/Input.cpp
    src/Graphics/Renderer.cpp
    src/World/World.cpp
    src/Graphics/Sprite.cpp
    src/Math/Vector2f.cpp
    src/Math/Vector2i.cpp
)

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

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(${PROJECT_NAME} 
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include   
)

target_sources(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)