#include "Input.hpp"

#include <unordered_map>

#include <SDL2/SDL.h>

static std::unordered_map<SDL_Scancode, InputButton> keyMap;
static bool buttonsPressed[32] = {0};
static bool buttonsPressedOld[32] = {0};

void Input::init()
{
    keyMap.insert({SDL_SCANCODE_W, BUTTON_UP});
    keyMap.insert({SDL_SCANCODE_A, BUTTON_LEFT});
    keyMap.insert({SDL_SCANCODE_S, BUTTON_DOWN});
    keyMap.insert({SDL_SCANCODE_D, BUTTON_RIGHT});
    keyMap.insert({SDL_SCANCODE_UP, BUTTON_UP});
    keyMap.insert({SDL_SCANCODE_LEFT, BUTTON_LEFT});
    keyMap.insert({SDL_SCANCODE_DOWN, BUTTON_DOWN});
    keyMap.insert({SDL_SCANCODE_RIGHT, BUTTON_RIGHT});
    keyMap.insert({SDL_SCANCODE_SPACE, BUTTON_INTERACT});
    keyMap.insert({SDL_SCANCODE_F, BUTTON_INTERACT2});
}

bool Input::isButtonDown(InputButton button)
{
    return buttonsPressed[button];
}

bool Input::isButtonPressed(InputButton button)
{
    return buttonsPressed[button] && !buttonsPressedOld[button];
}

void Input::update()
{
    std::copy(std::begin(buttonsPressed), std::end(buttonsPressed), std::begin(buttonsPressedOld));
}

void Input::handleInput(SDL_Event* p_event)
{
    bool keyPressed;

    if (p_event->type == SDL_KEYDOWN)
    {
        keyPressed = true;
    }
    else
    {
        keyPressed = false;
    }

    InputButton index = keyMap[p_event->key.keysym.scancode];
    buttonsPressed[index] = keyPressed;
}