#include "Vector2f.hpp"

#include <cmath>

Vector2f::Vector2f()
    : x(0), y(0) {}

Vector2f::Vector2f(float x, float y)
    : x(x), y(y) {}

float Vector2f::dot(const Vector2f& right) const
{
    return x * right.x + y * right.y;
}

float Vector2f::length() const
{
    return sqrtf(powf(x, 2) + powf(y, 2));
}

void Vector2f::normalize()
{
    float scale = 1.0f / length();
    x *= scale;
    y *= scale;
}

void Vector2f::operator =(const Vector2f& other)
{
    x = other.x;
    y = other.y;
}

void Vector2f::operator +=(const Vector2f& other)
{
    x += other.x;
    y += other.y;
}

void Vector2f::operator -=(const Vector2f& other)
{
    x -= other.x;
    y -= other.y;
}

void Vector2f::operator *=(float multiplier)
{
    x *= multiplier;
    y *= multiplier;
}

void Vector2f::operator *=(const Vector2f& other)
{
    x *= other.x;
    y *= other.y;
}

void Vector2f::operator /=(const Vector2f& other)
{
    x /= other.x;
    y /= other.y;
}

bool Vector2f::operator ==(const Vector2f& other) const
{
    return x == other.x && y == other.y;
}

Vector2f Vector2f::operator +(const Vector2f& other) const
{
    return Vector2f(x + other.x, y + other.y);
}

Vector2f Vector2f::operator -(const Vector2f& other) const
{
    return Vector2f(x - other.x, y - other.y);
}

Vector2f Vector2f::operator *(float multiplier) const
{
    return Vector2f(x * multiplier, y * multiplier);
}

Vector2f Vector2f::operator /(float divider) const
{
    return Vector2f(x / divider, y / divider);
}