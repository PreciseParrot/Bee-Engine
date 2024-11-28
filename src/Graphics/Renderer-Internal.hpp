#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Bee/Math/Vector2f.hpp"
#include "Bee/Math/Vector2i.hpp"

namespace Renderer
{
    void init(int windowWidth, int windowHeight);
    void update();
    void handleEvent(const SDL_Event* event);
    void drawTile(const Vector2i& position, const SDL_Rect* srcRect, SDL_Texture* texture);
    void drawHUD(const Vector2i& position, const Vector2i& scale, const SDL_Rect* srcRect, SDL_Texture* texture, const Vector2f& rotationCenter, float rotation);
    void drawSprite(const Vector2f& position, const Vector2f& scale, const SDL_Rect* srcRect, SDL_Texture* texture, const Vector2f& rotationCenter, float rotation);
    SDL_Texture* createTexture(SDL_Surface* surface);
    SDL_Texture* loadTexture(const std::string& textureName, const std::string& path);
    TTF_Font* loadFont(const std::string& font, int size);
    void cleanUp();
};