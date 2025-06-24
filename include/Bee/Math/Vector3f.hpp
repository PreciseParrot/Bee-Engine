#pragma once

#include "FWD.hpp"

struct Vector3f
{
    float x;
    float y;
    float z;

    Vector3f();
    Vector3f(float x, float y, float z);

    operator Vector2f() const;
    operator Vector2i() const;
    operator Vector3i() const;

    float dot(const Vector3f& other) const;
    float getLength() const;
    void normalize();

    void operator+=(const Vector3f& other);

    Vector3f operator+(const Vector3f& other) const;
};
