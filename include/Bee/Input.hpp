#pragma once

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
    bool isButtonDown(InputButton button);
    bool isButtonPressed(InputButton button);
};