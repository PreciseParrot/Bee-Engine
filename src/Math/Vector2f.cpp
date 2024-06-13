#include "Vector2f.hpp"

#include <cmath>

Vector2f::Vector2f()
    : x(0), y(0) {}

Vector2f::Vector2f(const float x, const float y)
    : x(x), y(y) {}

float Vector2f::dot(const Vector2f& right) const
{
    return x * right.x + y * right.y;
}

float Vector2f::getLength() const
{
    return sqrtf(powf(x, 2) + powf(y, 2));
}

void Vector2f::normalize()
{
    const float length = getLength();
    if (length <= 0) return;
    const float scale = 1.0f / length;
    x *= scale;
    y *= scale;
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

void Vector2f::operator *=(const float multiplier)
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
    return {x + other.x, y + other.y};
}

Vector2f Vector2f::operator -(const Vector2f& other) const
{
    return {x - other.x, y - other.y};
}

Vector2f Vector2f::operator *(const float multiplier) const
{
    return {x * multiplier, y * multiplier};
}

Vector2f Vector2f::operator /(const float divider) const
{
    return {x / divider, y / divider};
}