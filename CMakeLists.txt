cmake_minimum_required(VERSION 3.27)

project(bee VERSION 1)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/${CMAKE_PROJECT_NAME}/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE})

option(BEE_VENDORED "Use vendored libraries" OFF)
option(BEE_STATIC "Build as a static library" OFF)
option(BEE_STATIC_DEPENDENCIES "Build static libraries" OFF)

if (BEE_STATIC)
    add_library(${PROJECT_NAME} STATIC)
else ()
    add_library(${PROJECT_NAME} SHARED)
endif ()

target_sources(${PROJECT_NAME} PRIVATE
        src/Audio.cpp
        src/Bee.cpp
        src/Entity.cpp
        src/Log.cpp
        src/Properties.cpp

        src/Collision/Collision.cpp

        src/Graphics/Color.cpp
        src/Graphics/HUDObject.cpp
        src/Graphics/Renderer.cpp
        src/Graphics/Sprite.cpp
        src/Graphics/Window.cpp

        src/Graphics/Renderer/OpenGL/ErrorHandling.cpp
        src/Graphics/Renderer/OpenGL/IndexBuffer.cpp
        src/Graphics/Renderer/OpenGL/OpenGL.cpp
        src/Graphics/Renderer/OpenGL/Shader.cpp
        src/Graphics/Renderer/OpenGL/Texture.cpp
        src/Graphics/Renderer/OpenGL/VertexArray.cpp
        src/Graphics/Renderer/OpenGL/VertexBuffer.cpp
        src/Graphics/Renderer/OpenGL/VertexBufferLayout.cpp

        src/Input/Controller.cpp
        src/Input/Keyboard.cpp
        src/Input/Mouse.cpp

        src/World/World.cpp
        src/World/WorldObject.cpp

        src/Math/Math.cpp
        src/Math/Matrix4f.cpp
        src/Math/Vector2f.cpp
        src/Math/Vector2i.cpp
        src/Math/Vector3f.cpp
        src/Math/Vector3i.cpp
        src/Math/Vector4f.cpp
        src/Math/Vector4i.cpp
        src/Math/Quaternion.cpp
)

if (BEE_STATIC_DEPENDENCIES)
    set(BUILD_SHARED_LIBS OFF)
endif ()

if (BEE_VENDORED OR EMSCRIPTEN)
    if (NOT EMSCRIPTEN)
        set(SDL2MIXER_VENDORED ON)
        set(SDL2TTF_VENDORED ON)

        add_subdirectory(external/SDL2)
        add_subdirectory(external/SDL_mixer)
        add_subdirectory(external/SDL_ttf)

        set(ZLIB_BUILD_TESTING OFF CACHE BOOL "Enable Zlib Examples as tests" FORCE)
        if (BEE_STATIC_DEPENDENCIES)
            set(ZLIB_BUILD_SHARED OFF CACHE BOOL "Enable building zlib shared library" FORCE)
        endif ()
        add_subdirectory(external/zlib external/zlib-build EXCLUDE_FROM_ALL)

        if (BEE_STATIC_DEPENDENCIES)
            add_library(ZLIB::ZLIB ALIAS zlibstatic)
        endif ()

        set(ZLIB_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/external/zlib;${CMAKE_CURRENT_BINARY_DIR}/external/zlib-build")
        set(ZLIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/zlib;${CMAKE_CURRENT_BINARY_DIR}/external/zlib-build" CACHE STRING "path of zlib, passed to libpng" FORCE)
        set(ZLIB_LIBRARY ZLIB::ZLIB)

        set(PNG_TOOLS OFF CACHE BOOL "Build the libpng tools" FORCE)
        set(SKIP_INSTALL_EXPORT TRUE)
        add_subdirectory(external/libpng)
    endif ()
    add_subdirectory(external/json)
    add_subdirectory(external/tinyxml2)
else ()
    find_package(SDL2 REQUIRED)
    find_package(SDL2_ttf REQUIRED)
    find_package(SDL2_mixer REQUIRED)
    find_package(PNG REQUIRED)
    find_package(nlohmann_json REQUIRED)
    find_package(tinyxml2 REQUIRED)
endif()

if (EMSCRIPTEN)
    target_link_options(${PROJECT_NAME} PUBLIC
            -sUSE_SDL=2
            -sUSE_SDL_TTF=2
            -sUSE_SDL_MIXER=2
            -sUSE_LIBPNG
            -sUSE_VORBIS
            -sUSE_HARFBUZZ=1
            -sUSE_OGG=1
            -sUSE_FREETYPE=1
            -sMIN_WEBGL_VERSION=2
            -sMAX_WEBGL_VERSION=2
            -O3
            -flto
            -sASSERTIONS=0
            -sOFFSCREENCANVAS_SUPPORT=1
    )
    target_compile_options(${PROJECT_NAME} PUBLIC
            -sUSE_SDL=2
            -sUSE_SDL_TTF=2
            -sUSE_SDL_MIXER=2
            -sUSE_LIBPNG
            -sUSE_VORBIS
            -sUSE_HARFBUZZ=1
            -sUSE_OGG=1
            -sUSE_FREETYPE=1
            -O3
            -flto
    )
else ()
    if (BEE_STATIC_DEPENDENCIES AND BEE_VENDORED)
        target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2-static)
        target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_mixer::SDL2_mixer-static)
        target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf-static)
        target_link_libraries(${PROJECT_NAME} PRIVATE png_static)
    else ()
        target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
        target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_mixer::SDL2_mixer)
        target_link_libraries(${PROJECT_NAME} PRIVATE SDL2_ttf::SDL2_ttf)
        target_link_libraries(${PROJECT_NAME} PRIVATE png)
    endif ()
endif ()

add_subdirectory(external/glad)
add_subdirectory(external/imgui)

target_link_libraries(${PROJECT_NAME} PRIVATE tinyxml2::tinyxml2)
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)