#pragma once

#include "FWD.hpp"

struct Vector4f
{
    float x;
    float y;
    float z;
    float w;

    Vector4f();
    Vector4f(float x, float y, float z, float w);

    operator Vector2f() const;
    operator Vector4i() const;
    
    float& operator[](int);
    const float& operator[](int) const;
};
