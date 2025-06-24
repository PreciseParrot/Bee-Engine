#pragma once

#include "FWD.hpp"

struct Vector2f
{
    float x;
    float y;

    Vector2f();
    Vector2f(float x, float y);

    operator Vector2i() const;
    operator Vector3f() const;
    operator Vector4f() const;
    
    float dot(const Vector2f& other) const;
    float getLength() const;
    void normalize();

    Vector2f& operator=(const Vector3f& other);
    
    void operator+=(const Vector2f& other);
    void operator-=(const Vector2f& other);
    void operator*=(float multiplier);
    void operator*=(const Vector2f& other);
    void operator/=(float divider);
    void operator/=(const Vector2f& other);

    bool operator==(const Vector2f& other) const;

    Vector2f operator+(const Vector2f& other) const;
    Vector2f operator-() const;
    Vector2f operator-(const Vector2f& other) const;
    Vector2f operator*(float multiplier) const;
    Vector2f operator*(const Vector2f& other) const;
    Vector2f operator/(float divider) const;
    Vector2f operator/(const Vector2f& other) const;
};