/**
 * @file Renderer.hpp
 */

#pragma once

#include <string>

#include "Bee/Math/Vector2f.hpp"
#include "Bee/Math/Vector2i.hpp"

/**
 * @namespace Renderer
 * 
 * @brief All the renderer and window related functions.
 * 
 */
namespace Renderer
{
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
     * @brief Get the size of the internal screen. 
     * 
     * @return the size of the internal screen
     */
    Vector2i getScreenSize();

    /**
     * @brief Get the size of a texture
     * 
     * @return the size of the given texture
     */
    Vector2f getTextureSize(int textureID);

    /**
     * @brief Get the size of the viewport.
     * 
     * @return the size of the viewport
     */
    Vector2f getViewPortSize();

    /**
     * @brief Set the position of the camera
     * 
     * @param position the position of the camera in world coordinates
     */
    void setCameraPosition(const Vector2f& position);

    void setShader(const std::string& shader);

    /**
     * @brief Set the size of the viewport
     * 
     * @param size the size of the viewport
     */
    void setViewportSize(const Vector2f& size);

    void setOnFrameCB(void (*func)());

    void setUniform1f(const std::string& name, float data);
    void setUniform2f(const std::string& name, const Vector2f& data);
    void setUniform3f(const std::string& name, const Vector3f& data);
    void setUniform4f(const std::string& name, const Vector4f& data);
    void setUniformMat4f(const std::string& name, const Matrix4f& matrix);
};