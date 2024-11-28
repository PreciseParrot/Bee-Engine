#pragma once

#include <SDL2/SDL.h>

namespace Mouse
{
    void init();
    void handleInput(const SDL_Event* event);
    void handleMovement(const SDL_Event* event);
    void update();
    void cleanUp();
}