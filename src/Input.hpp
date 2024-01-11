#pragma once

#include <bitset>

#include <SDL2/SDL.h>

enum InputButton
{
    BUTTON_UP,
    BUTTON_LEFT,
    BUTTON_DOWN,
    BUTTON_RIGHT,
    BUTTON_INTERACT,
    BUTTON_INTERACT2
};

namespace Input
{
    void init();
    void handleInput(SDL_Event* p_event);
    void update();
    bool isButtonDown(InputButton button);
    bool isButtonPressed(InputButton button);
};