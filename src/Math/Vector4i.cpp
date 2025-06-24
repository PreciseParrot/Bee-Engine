#include "Bee/Math/Vector4i.hpp"

Vector4i::Vector4i()
{
    x = 0;
    y = 0;
    z = 0;
    w = 0;
}

Vector4i::Vector4i(const int x, const int y, const int z, const int w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}
