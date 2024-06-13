/**
 * @file Keyboard.hpp
 */

#pragma once

#include <SDL2/SDL.h>

#include "Input/Keys.hpp"

/**
 * @namespace Keyboard
 * 
 * @brief All the keyboard related functions.
 * 
 */
namespace Keyboard
{
    
    void init();
    void handleInput(const SDL_Event* event);
    void update();

    /*Internal functions end here*/

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