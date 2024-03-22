#pragma once

#include <SDL2/SDL.h>

#include "Input/Keys.hpp"

namespace Keyboard
{
    void init();
    void handleInput(SDL_Event* event);
    void update();
    bool isKeyDown(Key key);
    bool isKeyPressed(Key key);
}