#pragma once

#include <map>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Graphics/Sprite.hpp"
#include "Math/Vector2f.hpp"
#include "Math/Vector2i.hpp"

namespace Renderer
{
    void init(int windowWidth, int windowHeight);
    void update();
    void drawTile(const Vector2i& position, SDL_Rect* srcRect, SDL_Texture* texture);
    void drawHUD(const Vector2i& position, const Vector2i& scale, SDL_Rect* srcRect, SDL_Texture* texture, const Vector2f& rotationCenter, float rotation);
    void drawSprite(const Vector2f& position, const Vector2f& scale, SDL_Rect* srcRect, SDL_Texture* texture, const Vector2f& rotationCenter, float rotation);
    SDL_Texture* createTexture(SDL_Surface* surface);
    SDL_Texture* loadTexture(std::string textureName, std::string path);
    TTF_Font* loadFont(std::string font, int size);
    void unloadAllFonts();
    void unloadAllTextures();
    Vector2i getScreenSize();
    void setWindowIcon(std::string path);
    void setWindowTitle(std::string title);
    void setCameraPosition(float x, float y);
    void setCameraPosition(const Vector2f& cameraPosition);
    void setViewportSize(float width, float height);
    void setViewportSize(const Vector2f& viewportSize);
    void cleanUp();
};