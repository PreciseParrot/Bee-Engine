/**
 * @file Window.hpp
 */

#pragma once

#include <string>

#include "Bee/Math/Vector2i.hpp"

/**
 * @namespace Window
 * 
 * @brief All the renderer and window related functions.
 * 
 */
namespace Window
{
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
}