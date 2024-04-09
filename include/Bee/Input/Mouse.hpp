#pragma once

#include "Bee/Input/MouseButtons.hpp"
#include "Bee/Math/Vector2f.hpp"
#include "Bee/Math/Vector2i.hpp"

namespace Mouse
{
    bool isButtonDown(MouseButton button);
    bool isButtonPressed(MouseButton button);
    Vector2i getMouseScreenPosition();
    Vector2f getMouseWorldPosition();
}