#pragma once

#include <SDL2/SDL.h>

namespace Controller
{
    void init();
    void handleButtonInput(const SDL_Event* event);
    void connectController(const SDL_Event* event);
    void disconnectController(const SDL_Event* event);
    void update();
    void cleanUp();
}