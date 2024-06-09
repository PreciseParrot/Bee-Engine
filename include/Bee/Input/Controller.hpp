/**
 * @file Controller.hpp
 */

#pragma once

#include "Bee/Input/ControllerButtons.hpp"
#include "Bee/Math/Vector2f.hpp"

/**
 * @namespace Controller
 * 
 * @brief All the controller related functions.
 * 
 */
namespace Controller
{
    /**
     * @brief Checks if a button is pressed.
     * 
     * @param playerIndex the player index of the controller
     * @param button the button to check
     * @return true if the button is pressed.
     */
    bool isButtonDown(int playerIndex, ControllerButton button);

    /**
     * @brief Checks if a button is pressed but wasn't pressed in the previous frame.
     * 
     * @param playerIndex the player index of the controller
     * @param button the button to check
     * @return true if the is pressed but wasn't pressed in the previous frame.
     */
    bool isButtonPressed(int playerIndex, ControllerButton button);

    /**
     * @brief Get the state of the left analog stick.
     * 
     * @param playerIndex the player index of the controller
     * @return the position of the left analog stick in values ranging from -1 to 1.
     */
    Vector2f getLeftStick(int playerIndex);

    /**
     * @brief Get the state of the right analog stick.
     * 
     * @param playerIndex the player index of the controller
     * @return the position of the right analog stick in values ranging from -1 to 1.
     */
    Vector2f getRightStick(int playerIndex);

    /**
     * @brief Get the state of the left analog trigger.
     * 
     * @param playerIndex the player index of the controller
     * @return the position of the left analog trigger ranging from 0 to 1.
     */
    float getLeftTrigger(int playerIndex);

    /**
     * @brief Get the state of the right analog trigger.
     * 
     * @param playerIndex the player index of the controller
     * @return the position of the right analog trigger ranging from 0 to 1.
     */
    float getRightTrigger(int playerIndex);
}