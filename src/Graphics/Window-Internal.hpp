#pragma once

#include <SDL2/SDL.h>

namespace Window
{
    void init(int windowWidth, int windowHeight);
    void cleanUp();
    SDL_Window* getWindow();
}