/**
 * @file Controller.hpp
 */

#pragma once

#include <SDL2/SDL.h>

#include "Input/ControllerButtons.hpp"
#include "Math/Vector2f.hpp"

/**
 * @namespace Controller
 * 
 * @brief All the controller related functions.
 * 
 */
namespace Controller
{
    /*Internal functions start here*/

    void init();
    void handleInput(SDL_Event* event);
    void connectController(SDL_Event* event);
    void disconnectController(SDL_Event* event);
    void update();
    void cleanUp();

    /*Internal functions end here*/

    /**
     * @brief Checks if a button is pressed.
     * 
     * @param button the button to check
     * @return true if the button is pressed.
     */
    bool isButtonDown(ControllerButton button);

    /**
     * @brief Checks if a button is pressed but wasn't pressed in the previous frame.
     * 
     * @param button the button to check
     * @return true if the is pressed but wasn't pressed in the previous frame.
     */
    bool isButtonPressed(ControllerButton button);

    /**
     * @brief Get the state of the left analog stick.
     * 
     * @return the position of the left analog stick in values ranging from -1 to 1.
     */
    Vector2f getLeftStick();

    /**
     * @brief Get the state of the right analog stick.
     * 
     * @return the position of the right analog stick in values ranging from -1 to 1.
     */
    Vector2f getRightStick();
}