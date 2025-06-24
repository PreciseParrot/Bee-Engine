#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Bee/Entity.hpp"
#include "Bee/Graphics/HUDObject.hpp"
#include "Bee/Math/Vector2f.hpp"
#include "Graphics/Rect.hpp"

namespace Renderer
{
    void init(int windowWidth, int windowHeight);
    void update();
    void handleEvent(const SDL_Event* event);
    void queueTile(const Vector3f& position, int textureID, const Rect& rect);
    void queueHUD(const Vector3f& position, const Vector2f& scale, int shaderID, int textureID, const Rect& rect, HUDObject* hudObject);
    void queueEntity(const Vector3f& position, const Vector2f& scale, int shaderID, int textureID, const Rect& rect, Entity* entity);
    int loadShader(const std::string& shader);
    int loadTexture(const std::string& textureName, const std::string& path);
    int createUniqueTexture(const SDL_Surface* surface);
    SDL_Surface* loadSurface(const std::string& path);
    TTF_Font* loadFont(const std::string& font, int size);
    void deleteUniqueTexture(int textureID);
    void cleanUp();
}