#pragma once

#include <map>
#include <string>

#include <SDL2/SDL.h>

#include "Graphics/Sprite.hpp"
#include "Math/Vector2f.hpp"
#include "Math/Vector2i.hpp"

namespace Renderer
{
    void init();
    void clear();
    void display();
    void draw(const Vector2i& position, SDL_Rect* p_srcRect, SDL_Texture* p_texture);
    void draw(const Vector2f& position, SDL_Rect* p_srcRect, SDL_Texture* p_texture, const Vector2f& scale, Vector2f& rotationCenter, float rotation);
    SDL_Texture* loadTexture(std::string textureName, std::string path);
    void unloadTexture(std::string textureName);
    void unloadAllTextures();
    void setCameraPosition(float x, float y);
    void setCameraPosition(const Vector2f& cameraPosition);
    void setViewportSize(float width, float height);
    void setViewportSize(const Vector2f& viewportSize);
    void cleanUp();
};