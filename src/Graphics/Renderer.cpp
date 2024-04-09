#include "Renderer.hpp"

#include <cmath>
#include <exception>
#include <filesystem>
#include <map>
#include <unordered_map>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Log.hpp"
#include "Math/Vector2f.hpp"
#include "Math/Vector2i.hpp"

static int windowWidth = 0;
static int windowHeight = 0;
static int screenWidth = 0;
static int screenHeight = 0;
static float viewPortWidth = 16.0f;
static float viewPortHeight = 9.0f;
static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static SDL_Texture* targetTexture = nullptr;
static std::unordered_map<std::string, SDL_Texture*> textureMap;
static std::map<std::pair<std::string, int>, TTF_Font*> fontMap;
static Vector2f cameraPosition;

void Renderer::init(int winWidth, int winHeight)
{
    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        Log::write("Renderer", LogLevel::Error, "Error initializing SDL2_image: ", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() == -1)
    {
        Log::write("Renderer", LogLevel::Error, "Error initializing SDL2_ttf: ", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("Bee Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winWidth, winHeight, 0);
    if (window == nullptr)
    {
        Log::write("Renderer", LogLevel::Error, "Error creating Window: ", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
    {
        Log::write("Renderer", LogLevel::Error, "Error creating renderer: ", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_SetWindowResizable(window, SDL_TRUE);

    Log::write("Renderer", LogLevel::Info, "Initialized renderer");
}

void Renderer::update()
{
    int width;
    int height;

    SDL_GetWindowSize(window, &width, &height);

    if (width != windowWidth || height != windowHeight)
    {
        windowWidth = width;
        windowHeight = height;

        float widthFactor = windowWidth / viewPortWidth;
        float heightFactor = windowHeight / viewPortHeight;

        if (widthFactor > heightFactor)
        {
            screenWidth = windowWidth * heightFactor / widthFactor;
            screenHeight = windowHeight;
        }
        else
        {
            screenWidth = windowWidth;
            screenHeight = windowHeight * widthFactor / heightFactor;
        }

        SDL_SetRenderTarget(renderer, NULL);
        SDL_DestroyTexture(targetTexture);
        targetTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);
        SDL_SetRenderTarget(renderer, targetTexture);
    }

    SDL_Rect dstRect;
    dstRect.x = (windowWidth - screenWidth) / 2;
    dstRect.y = (windowHeight - screenHeight) / 2;
    dstRect.w = screenWidth;
    dstRect.h = screenHeight;

    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, targetTexture, NULL, &dstRect);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, targetTexture);
    SDL_RenderClear(renderer);
}

void Renderer::drawTile(const Vector2i& position, SDL_Rect* srcRect, SDL_Texture* texture)
{
    SDL_Rect dstRect;
    dstRect.x = ceilf((position.x - cameraPosition.x + viewPortWidth / 2) * screenWidth / viewPortWidth);
    dstRect.y = ceilf((position.y - cameraPosition.y + viewPortHeight / 2) * screenHeight / viewPortHeight);
    dstRect.h = ceilf(screenHeight / viewPortHeight);
    dstRect.w = ceilf(screenWidth / viewPortWidth);

    SDL_RenderCopy(renderer, texture, srcRect, &dstRect);
}

void Renderer::drawHUD(const Vector2i& position, const Vector2i& scale, SDL_Rect* srcRect, SDL_Texture* texture, const Vector2f& rotationCenter, float rotation)
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

void Renderer::drawSprite(const Vector2f& position, const Vector2f& scale, SDL_Rect* srcRect, SDL_Texture* texture, const Vector2f& rotationCenter, float rotation)
{
    SDL_Rect dstRect;
    dstRect.x = (position.x - scale.x / 2 - cameraPosition.x + viewPortWidth / 2) * screenWidth / viewPortWidth;
    dstRect.y = (position.y - scale.y / 2 - cameraPosition.y + viewPortHeight / 2) * screenHeight / viewPortHeight;
    dstRect.w = ceilf(screenWidth / viewPortWidth * scale.x);
    dstRect.h = ceilf(screenHeight / viewPortHeight * scale.y);

    SDL_Point centerPoint;
    centerPoint.x = dstRect.w * rotationCenter.x;
    centerPoint.y = dstRect.h * rotationCenter.y;

    SDL_RenderCopyEx(renderer, texture, srcRect, &dstRect, rotation, &centerPoint, SDL_FLIP_NONE);
}

SDL_Texture* Renderer::createTexture(SDL_Surface* surface)
{
    return SDL_CreateTextureFromSurface(renderer, surface);
}

SDL_Texture* Renderer::loadTexture(std::string textureName, std::string path)
{
    SDL_Texture* texture = nullptr;

    if (textureMap.find(textureName) != textureMap.end())
        return textureMap[textureName];
    
    texture = IMG_LoadTexture(renderer, path.c_str());
    if (texture == nullptr)
    {
        Log::write("Renderer", LogLevel::Warning, "Can't load texture: ", SDL_GetError());
    }
    else
    {
        textureMap.insert({textureName, texture});
        Log::write("Renderer", LogLevel::Info, "Loaded " + textureName + " texture");
    }
    return texture;
}

TTF_Font* Renderer::loadFont(std::string fontName, int size)
{
    if (fontMap.find({fontName, size}) != fontMap.end())
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
        Log::write("Renderer", LogLevel::Warning, "Can't load " + fontName + " font: ", SDL_GetError());
    }
    else
    {
        Log::write("Renderer", LogLevel::Info, "Loaded " + fontName + " font with size " + std::to_string(size));
        fontMap.insert({{fontName, size}, font});
    }
    return font;
}

void Renderer::unloadAllFonts()
{
    for (const auto& [key, font] : fontMap)
    {
        Log::write("Renderer", LogLevel::Info, "Unloaded " + key.first + " font with size " + std::to_string(key.second));
        TTF_CloseFont(font);
    }
    fontMap.clear();
}

void Renderer::unloadAllTextures()
{
    for (const auto& [name, texture] : textureMap)
    {
        Log::write("Renderer", LogLevel::Info, "Unloaded " + name + " texture");
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
    return Vector2f(viewPortWidth, viewPortHeight);
}

Vector2i Renderer::getScreenSize()
{
    return Vector2i(screenWidth, screenHeight);
}

void Renderer::setWindowIcon(std::string path)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr)
    {
        Log::write("Renderer", LogLevel::Warning, "Can't load image: ", SDL_GetError());
        return;
    }
    SDL_SetWindowIcon(window, surface);
    SDL_FreeSurface(surface);
}

void Renderer::setWindowTitle(std::string title)
{
    SDL_SetWindowTitle(window, title.c_str());
}

void Renderer::setCameraPosition(float x, float y)
{
    cameraPosition.x = x;
    cameraPosition.y = y;
}

void Renderer::setCameraPosition(const Vector2f& newCameraPosition)
{
    cameraPosition = newCameraPosition;
}

void Renderer::setViewportSize(float width, float height)
{
    viewPortWidth = width;  
    viewPortHeight = height;
}

void Renderer::setViewportSize(const Vector2f& viewportSize)
{
    viewPortWidth = viewportSize.x;
    viewPortHeight = viewportSize.y;  
}

void Renderer::cleanUp()
{
    unloadAllTextures();
    SDL_DestroyTexture(targetTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
}