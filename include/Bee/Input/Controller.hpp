#pragma once

#include "Bee/Input/ControllerButtons.hpp"
#include "Bee/Math/Vector2f.hpp"

namespace Controller
{
    bool isButtonDown(ControllerButton button);
    bool isButtonPressed(ControllerButton button);
    Vector2f getLeftStick();
    Vector2f getRightStick();
    void setDeadzone(Vector2f deadzone);
}