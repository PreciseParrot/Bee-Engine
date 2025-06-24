#pragma once

#include "FWD.hpp"

struct Vector2i
{
    int x;
    int y;

    Vector2i();
    Vector2i(int x, int y);

    operator Vector2f() const;
    operator Vector3i() const;
    operator Vector4i() const;

    int dot() const;
    float getLength() const;

    void operator+=(const Vector2i& other);
    void operator-=(const Vector2i& other);
    void operator*=(float multiplier);
    void operator*=(const Vector2i& other);
    void operator/=(float divider);
    void operator/=(const Vector2i& other);
    
    bool operator==(const Vector2i& other) const;

    Vector2i operator+(const Vector2i& other) const;
    Vector2i operator-(const Vector2i& other) const;
    Vector2i operator*(float multiplier) const;
    Vector2i operator*(const Vector2i& other) const;
    Vector2i operator/(float divider) const;
    Vector2i operator/(const Vector2i& other) const;
};