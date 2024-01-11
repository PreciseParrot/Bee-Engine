#include "Renderer.hpp"

#include <cmath>
#include <exception>
#include <iostream>
#include <unordered_map>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Math/Vector2f.hpp"
#include "Math/Vector2i.hpp"

static int windowWidth = 0;
static int windowHeight = 0;
static float viewPortWidth = 16.0f;
static float viewPortHeight = 9.0f;
static SDL_Window* p_window;
static SDL_Renderer* p_renderer;
static std::unordered_map<std::string, SDL_Texture*> textureMap;
static Vector2f cameraPosition;

void Renderer::init(int wWidth, int wHeight)
{
    windowWidth = wWidth;
    windowHeight = wHeight;

    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        std::cout << "Error initializing SDL2_image: " << SDL_GetError() << std::endl;
        throw std::exception();
    }

    p_window = SDL_CreateWindow("GAEM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0);
    if (p_window == nullptr)
    {
        std::cout << "Error creating Window: " << SDL_GetError() << std::endl;
        throw std::exception();
    }

    p_renderer = SDL_CreateRenderer(p_window, -1, 0);
    if (p_renderer == nullptr)
    {
        std::cout << "Error creating renderer: " << SDL_GetError() << std::endl;
        throw std::exception();
    }

    std::cout << "Initialized renderer" << std::endl;
}

void Renderer::clear()
{
    SDL_RenderClear(p_renderer);
}

void Renderer::display()
{
    SDL_RenderPresent(p_renderer);
}

void Renderer::draw(const Vector2i& position, SDL_Rect* p_srcRect, SDL_Texture* p_texture)
{
    SDL_Rect dst;
    dst.x = ceilf((position.x - cameraPosition.x + viewPortWidth / 2) * windowWidth / viewPortWidth);
    dst.y = ceilf((position.y - cameraPosition.y + viewPortHeight / 2) * windowHeight / viewPortHeight);
    dst.h = ceilf(windowHeight / viewPortHeight);
    dst.w = ceilf(windowWidth / viewPortWidth);

    SDL_RenderCopy(p_renderer, p_texture, p_srcRect, &dst);
}

void Renderer::draw(const Vector2f& position, SDL_Rect* p_srcRect, SDL_Texture* p_texture, const Vector2f& scale, Vector2f& rotationCenter, float rotation)
{
    SDL_Rect dst;
    dst.x = (position.x - scale.x / 2 - cameraPosition.x + viewPortWidth / 2) * windowWidth / viewPortWidth;
    dst.y = (position.y - scale.y / 2 - cameraPosition.y + viewPortHeight / 2) * windowHeight / viewPortHeight;
    dst.h = windowHeight / viewPortHeight * scale.x;
    dst.w = windowWidth / viewPortWidth * scale.y;

    SDL_Point centerPoint;
    centerPoint.x = dst.w * rotationCenter.x;
    centerPoint.y = dst.h * rotationCenter.y;

    SDL_RenderCopyEx(p_renderer, p_texture, p_srcRect, &dst, rotation, &centerPoint, SDL_FLIP_NONE);
}

SDL_Texture* Renderer::loadTexture(std::string textureName, std::string path)
{
    SDL_Texture* p_texture = nullptr;

    if (textureMap.find(textureName) != textureMap.end())
        return textureMap[textureName];
    
    SDL_Surface* p_surface = IMG_Load(path.c_str());
    if (p_surface == nullptr)
    {
        std::cout << "Error loading image" << std::endl;
        throw std::exception();
    }

    p_texture = SDL_CreateTextureFromSurface(p_renderer, p_surface);
    if (p_texture == nullptr)
    {
        std::cout << "Error creating texture: " << SDL_GetError() << std::endl;
        throw std::exception();
    }
    SDL_FreeSurface(p_surface);

    textureMap.insert(std::pair<std::string, SDL_Texture*>(textureName, p_texture));
    return p_texture;
}

void Renderer::unloadTexture(std::string textureName)
{
    SDL_DestroyTexture(textureMap.at(textureName));
    textureMap.erase(textureName);
}

void Renderer::unloadAllTextures()
{
    for (const auto& [key, val] : textureMap)
    {
        std::cout << "Unloaded " << key << " texture" << std::endl;
        SDL_DestroyTexture(val);
    }
    textureMap.clear();
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
    SDL_DestroyRenderer(p_renderer);
    SDL_DestroyWindow(p_window);
    IMG_Quit();
}