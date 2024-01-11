#pragma once

#include "bee/Math/Vector2f.hpp"
#include "bee/Math/Vector2i.hpp"

namespace Renderer
{
    void setCameraPosition(float x, float y);
    void setCameraPosition(const Vector2f& cameraPosition);
    void setViewportSize(float width, float height);
    void setViewportSize(const Vector2f& viewportSize);
};