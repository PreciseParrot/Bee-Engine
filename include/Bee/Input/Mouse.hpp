/**
 * @file Mouse.hpp
 */

#pragma once

#include <string>

#include "Bee/Input/MouseButtons.hpp"
#include "Bee/Math/Vector2f.hpp"
#include "Bee/Math/Vector2i.hpp"

/**
 * @namespace Mouse
 * 
 * @brief All the mouse related functions.
 * 
 */
namespace Mouse
{
    /**
     * @brief Checks if a mouse button is pressed.
     * 
     * @param button the button to check
     * @return true if the button is pressed.
     */
    bool isButtonDown(MouseButton button);

    /**
     * @brief Checks if a mouse button is pressed but wasn't pressed in the previous frame.
     * 
     * @param button the button to check
     * @return true if the is pressed but wasn't pressed in the previous frame.
     */
    bool isButtonPressed(MouseButton button);

    /**
     * @brief Get the position of the mouse.
     * 
     * @return the position of the mouse in screen coordinates.
     */
    Vector2i getMouseScreenPosition();

    /**
     * @brief Get the position of the mouse.
     * 
     * @return the position of the mouse in world coordinates.
     */
    Vector2f getMouseWorldPosition();

    /**
     * @brief Create a custom mouse cursor.
     * 
     * @param path a file path from where to load the image from
     * @param hotX the X hotspot of the cursor
     * @param hotY the Y hotspot of the cursor
     */
    void createCustomCursor(const std::string& path, int hotX, int hotY);
}