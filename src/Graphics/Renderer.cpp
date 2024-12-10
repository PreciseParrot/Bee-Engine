#include "Bee/Graphics/Renderer.hpp"
#include "Renderer-Internal.hpp"

#include <cstdio>
#include <filesystem>
#include <map>
#include <unordered_map>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <png.h>

#include "Bee/Log.hpp"
#include "Bee/Math/Vector2f.hpp"
#include "Bee/Math/Vector2i.hpp"

static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static SDL_Texture* targetTexture = nullptr;
static std::map<std::pair<std::string, int>, TTF_Font*> fontMap;
static std::unordered_map<std::string, SDL_Texture*> textureMap;
static Vector2f cameraPosition;
static Vector2f viewportSize(16.0f, 9.0f);
static Vector2i screenSize;
static Vector2i windowSize;

void Renderer::init(const int windowWidth, const int windowHeight)
{
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        Log::write("Renderer", LogLevel::error, "Error initializing video system: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() == -1)
    {
        Log::write("Renderer", LogLevel::error, "Error initializing SDL2_ttf: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("Bee Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0);
    if (window == nullptr)
    {
        Log::write("Renderer", LogLevel::error, "Error creating Window: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        Log::write("Renderer", LogLevel::error, "Error creating renderer: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_SetWindowResizable(window, SDL_TRUE);

    Log::write("Renderer", LogLevel::info, "Initialized renderer");
}

void Renderer::update()
{
    SDL_Rect dstRect;
    dstRect.x = (windowSize.x - screenSize.x) / 2;
    dstRect.y = (windowSize.y - screenSize.y) / 2;
    dstRect.w = screenSize.x;
    dstRect.h = screenSize.y;

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, targetTexture, nullptr, &dstRect);
    SDL_RenderPresent(renderer);
    SDL_SetRenderTarget(renderer, targetTexture);
    SDL_RenderClear(renderer);
}

void Renderer::handleEvent(const SDL_Event* event)
{
    if (event->window.event == SDL_WINDOWEVENT_RESIZED)
    {
        windowSize.x = event->window.data1;
        windowSize.y = event->window.data2;

        const float widthFactor = windowSize.x / viewportSize.x;
        const float heightFactor = windowSize.y / viewportSize.y;

        if (widthFactor > heightFactor)
        {
            screenSize.x = windowSize.x * heightFactor / widthFactor;
            screenSize.y = windowSize.y;
        }
        else
        {
            screenSize.x = windowSize.x;
            screenSize.y = windowSize.y * widthFactor / heightFactor;
        }

        SDL_SetRenderTarget(renderer, nullptr);
        SDL_DestroyTexture(targetTexture);
        targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, screenSize.x, screenSize.y);
        SDL_RenderClear(renderer);
        SDL_SetRenderTarget(renderer, targetTexture);
    }
}

void Renderer::drawTile(const Vector2i& position, const SDL_Rect* srcRect, SDL_Texture* texture)
{
    SDL_FRect dstRect;
    dstRect.x = (position.x - cameraPosition.x + viewportSize.x / 2) * screenSize.x / viewportSize.x;
    dstRect.y = (position.y - cameraPosition.y + viewportSize.y / 2) * screenSize.y / viewportSize.y;
    dstRect.h = screenSize.y / viewportSize.y + 0.04f;
    dstRect.w = screenSize.x / viewportSize.x + 0.04f;

    SDL_RenderCopyF(renderer, texture, srcRect, &dstRect);
}

void Renderer::drawHUD(const Vector2i& position, const Vector2i& scale, const SDL_Rect* srcRect, SDL_Texture* texture, const Vector2f& rotationCenter, const float rotation)
{
    SDL_Rect dstRect;
    dstRect.x = position.x;
    dstRect.y = position.y;
    dstRect.w = scale.x;
    dstRect.h = scale.y;

    SDL_Point centerPoint;
    centerPoint.x = dstRect.w * rotationCenter.x;
    centerPoint.y = dstRect.h * rotationCenter.y;

    SDL_RenderCopyEx(renderer, texture, srcRect, &dstRect, rotation, &centerPoint, SDL_FLIP_NONE);
}

void Renderer::drawSprite(const Vector2f& position, const Vector2f& scale, const SDL_Rect* srcRect, SDL_Texture* texture, const Vector2f& rotationCenter, const float rotation)
{
    SDL_FRect dstRect;
    dstRect.x = (position.x - scale.x / 2 - cameraPosition.x + viewportSize.x / 2) * screenSize.x / viewportSize.x;
    dstRect.y = (position.y - scale.y / 2 - cameraPosition.y + viewportSize.y / 2) * screenSize.y / viewportSize.y;
    dstRect.w = screenSize.x / viewportSize.x * scale.x;
    dstRect.h = screenSize.y / viewportSize.y * scale.y;

    SDL_FPoint centerPoint;
    centerPoint.x = dstRect.w * rotationCenter.x;
    centerPoint.y = dstRect.h * rotationCenter.y;

    SDL_RenderCopyExF(renderer, texture, srcRect, &dstRect, rotation, &centerPoint, SDL_FLIP_NONE);
}

SDL_Texture* Renderer::createTexture(SDL_Surface* surface)
{
    return SDL_CreateTextureFromSurface(renderer, surface);
}

SDL_Texture* Renderer::loadTexture(const std::string& textureName, const std::string& path)
{
    if (textureMap.contains(textureName))
        return textureMap[textureName];

    SDL_Surface* surface = loadSurface(path);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr)
    {
        Log::write("Renderer", LogLevel::error, "Can't load texture: %s / %s", textureName.c_str(), SDL_GetError());
    }
    else
    {
        textureMap.insert({textureName, texture});
        Log::write("Renderer", LogLevel::info, "Loaded %s texture", textureName.c_str());
    }

    SDL_FreeSurface(surface);
    return texture;
}

SDL_Surface* Renderer::loadSurface(const std::string& path)
{
    FILE* file = nullptr;
    png_structp png = nullptr;
    png_infop info = nullptr;

    file  = fopen(path.c_str(), "rb");
    if (!file)
    {
        Log::write("Renderer", LogLevel::warning, "Can't load texture: %s / file not found", path.c_str());
        fclose(file);    
    }

    png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png)
    {
        Log::write("Renderer", LogLevel::warning, "Can't load texture: %s / error parsing data", path.c_str());
        fclose(file);
        png_destroy_read_struct(&png, nullptr, nullptr);
    }

    info = png_create_info_struct(png);
    if (!info)
    {
        fclose(file);
        Log::write("Renderer", LogLevel::warning, "Can't load texture: %s / error parsing data", path.c_str());
        png_destroy_read_struct(&png, nullptr, nullptr);
    }

    if (setjmp(png_jmpbuf(png)))
    {
        fclose(file);
        png_destroy_read_struct(&png, nullptr, nullptr);
    }

    png_init_io(png, file);
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    unsigned char* data = new unsigned char[png_get_rowbytes(png, info) * height];
    png_bytep* row_pointers = new png_bytep[height];

    for (int y = 0; y < height; y++)
    {
        row_pointers[y] = data + y * png_get_rowbytes(png, info);
    }

    png_read_image(png, row_pointers);
    png_destroy_read_struct(&png, &info, nullptr);
    fclose(file);
    delete[] row_pointers;

    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(data, width, height, 32, width * 4, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
    
    return surface;
}

TTF_Font* Renderer::loadFont(const std::string& fontName, int size)
{
    if (fontMap.contains({fontName, size}))
    {
        return fontMap[{fontName, size}];
    }

    std::string path = "./assets/Fonts/" + fontName + ".ttf";

    if (!std::filesystem::exists(path))
    {
        path = "./assets/Fonts/" + fontName + ".otf";
    }

    TTF_Font* font = TTF_OpenFont(path.c_str(), size);

    if (font == nullptr)
    {
        Log::write("Renderer", LogLevel::error, "Can't load font: %s / %s", fontName.c_str(), SDL_GetError());
    }
    else
    {
        Log::write("Renderer", LogLevel::info, "Loaded %s font with size %i", fontName.c_str(), size);
        fontMap.insert({{fontName, size}, font});
    }
    return font;
}

void Renderer::unloadAllFonts()
{
    for (const auto& [key, font] : fontMap)
    {
        Log::write("Renderer", LogLevel::info, "Unloaded %s font with size %i", key.first.c_str(), key.second);
        TTF_CloseFont(font);
    }
    fontMap.clear();
}

void Renderer::unloadAllTextures()
{
    for (const auto& [textureName, texture] : textureMap)
    {
        Log::write("Renderer", LogLevel::info, "Unloaded %s texture", textureName.c_str());
        SDL_DestroyTexture(texture);
    }
    textureMap.clear();
}

Vector2f Renderer::getCameraPosition()
{
    return cameraPosition;
}

Vector2f Renderer::getViewPortSize()
{
    return viewportSize;
}

Vector2i Renderer::getScreenSize()
{
    return screenSize;
}

Vector2i Renderer::getWindowSize()
{
    return windowSize;
}

void Renderer::setFullscreen(const bool fullscreen)
{
    if (fullscreen)
    {
        SDL_Rect displaySize;
        SDL_Event event;
        SDL_GetDisplayBounds(SDL_GetWindowDisplayIndex(window), &displaySize);
        event.window.event = SDL_WINDOWEVENT_RESIZED;
        event.window.data1 = displaySize.w;
        event.window.data2 = displaySize.h;
        handleEvent(&event);

        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    else
    {
        SDL_SetWindowFullscreen(window, 0);
    }
}

void Renderer::setWindowIcon(const std::string& path)
{
    SDL_Surface* surface = loadSurface(path);
    if (surface == nullptr)
    {
        Log::write("Renderer", LogLevel::error, "Can't load image: %s", SDL_GetError());
        return;
    }
    SDL_SetWindowIcon(window, surface);
    SDL_FreeSurface(surface);
}

void Renderer::setWindowTitle(const std::string& title)
{
    SDL_SetWindowTitle(window, title.c_str());
}

void Renderer::setCameraPosition(const float x, const float y)
{
    cameraPosition.x = x;
    cameraPosition.y = y;
}

void Renderer::setCameraPosition(const Vector2f& position)
{
    cameraPosition = position;
}

void Renderer::setViewportSize(const float width, const float height)
{
    viewportSize.x = width;
    viewportSize.y = height;
}

void Renderer::setViewportSize(const Vector2f& size)
{
    viewportSize = size;
}

void Renderer::cleanUp()
{
    unloadAllFonts();
    unloadAllTextures();
    SDL_DestroyTexture(targetTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
}