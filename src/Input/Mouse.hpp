#pragma once

#include <SDL2/SDL.h>

#include "Input/MouseButtons.hpp"
#include "Math/Vector2f.hpp"
#include "Math/Vector2i.hpp"

namespace Mouse
{
    void init();
    void handleInput(SDL_Event* event);
    void update();
    bool isButtonDown(MouseButton button);
    bool isButtonPressed(MouseButton button);
    Vector2i getMouseScreenPosition();
    Vector2f getMouseWorldPosition();
}