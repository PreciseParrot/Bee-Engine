#include "Bee/Math/Vector4f.hpp"

#include "Bee/Math/Vector2f.hpp"
#include "Bee/Math/Vector3f.hpp"
#include "Bee/Math/Vector4i.hpp"

Vector4f::Vector4f()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 0.0f;
}

Vector4f::Vector4f(const float x, const float y, const float z, const float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector4f::operator Vector2f() const
{
    return {x, y};
}

Vector4f::operator Vector4i() const
{
    return { static_cast<int>(x), static_cast<int>(y), static_cast<int>(z), static_cast<int>(w)};
}

float& Vector4f::operator[](int index)
{
    switch (index)
    {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        default: return x;
    }
}

const float& Vector4f::operator[](int index) const
{
    switch (index)
    {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        default: return x;
    }
}
