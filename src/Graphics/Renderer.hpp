/**
 * @file Renderer.hpp
 */

#pragma once

#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Graphics/Sprite.hpp"
#include "Math/Vector2f.hpp"
#include "Math/Vector2i.hpp"

/**
 * @namespace Renderer
 * 
 * @brief All the renderer and window related functions.
 * 
 */
namespace Renderer
{
    /*Internal functions start here*/

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

    /*Internal functions end here*/


    /**
     * @brief Unload all loaded fonts.
     * 
     */
    void unloadAllFonts();

    /**
     * @brief Unload all loaded textures.
     * 
     */
    void unloadAllTextures();

    /**
     * @brief Get the position of the camera in the world.
     * 
     * @return the camera position in world coordinates
     */
    Vector2f getCameraPosition();

    /**
     * @brief Get the size of the viewport.
     * 
     * @return the size of the viewport
     */
    Vector2f getViewPortSize();

    /**
     * @brief Get the size of the internal screen. 
     * 
     * @return the size of the internal screen
     */
    Vector2i getScreenSize();

    /**
     * @brief Get the size of the window
     * 
     * @return the size of the window
     */
    Vector2i getWindowSize();

    /**
     * @brief Set the window to fullscreen or windowed mode.
     * 
     * @param fullscreen true for fullscreen and false for windowed mode
     */
    void setFullscreen(bool fullscreen);

    /**
     * @brief Set the icon of the window.
     * 
     * @param path a file path from where to load the image from
     */
    void setWindowIcon(const std::string& path);

    /**
     * @brief Set the title of the Window
     * 
     * @param title the title of the window
     */
    void setWindowTitle(const std::string& title);

    /**
     * @brief Set the position of the camera
     * 
     * @param x the x position of the camera in world coordinates
     * @param y the y position of the camera in world coordinates
     */
    void setCameraPosition(float x, float y);

    /**
     * @brief Set the position of the camera
     * 
     * @param cameraPosition the position of the camera in world coordinates
     */
    void setCameraPosition(const Vector2f& cameraPosition);

    /**
     * @brief Set the size of the viewport
     * 
     * @param width the width of the viewport
     * @param height the height of the viewport
     */
    void setViewportSize(float width, float height);

    /**
     * @brief Set the size of the viewport
     * 
     * @param viewportSize the size of the viewport
     */
    void setViewportSize(const Vector2f& viewportSize);
};