#pragma once

#include <SDL2/SDL.h>

namespace Keyboard
{
    void init();
    void handleInput(const SDL_Event* event);
    void update();
}