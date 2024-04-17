/**
 * @file Keyboard.hpp
 */

#pragma once

#include "Bee/Input/Keys.hpp"

/**
 * @namespace Keyboard
 * 
 * @brief All the keyboard related functions.
 * 
 */
namespace Keyboard
{
    /**
     * @brief Checks if a key is pressed.
     * 
     * @param key the key to check
     * @return true if the key is pressed.
     */
    bool isKeyDown(Key key);

    /**
     * @brief Checks if a key is pressed but wasn't pressed in the previous frame.
     * 
     * @param key the key to check
     * @return true if the is pressed but wasn't pressed in the previous frame.
     */
    bool isKeyPressed(Key key);
}