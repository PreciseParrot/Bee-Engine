#pragma once

#include <SDL2/SDL.h>

#include "Input/ControllerButtons.hpp"
#include "Math/Vector2f.hpp"

namespace Controller
{
    void init();
    void handleInput(SDL_Event* event);
    void connectController(SDL_Event* event);
    void disconnectController(SDL_Event* event);
    void update();
    void cleanUp();
    bool isButtonDown(ControllerButton button);
    bool isButtonPressed(ControllerButton button);
    Vector2f getLeftStick();
    Vector2f getRightStick();
    void setDeadzone(Vector2f deadzone);
}