#include "Vector2i.hpp"

Vector2i::Vector2i()
    : x(0), y(0) {}

Vector2i::Vector2i(const int x, const int y)
    : x(x), y(y) {}

void Vector2i::operator+=(const Vector2i& other)
{
    x += other.x;
    y += other.y;
}

void Vector2i::operator-=(const Vector2i& other)
{
    x -= other.x;
    y -= other.y;
}

bool Vector2i::operator==(const Vector2i& other) const
{
    return x == other.x && y == other.y;
}

Vector2i Vector2i::operator+(const Vector2i& other) const
{
    return {x + other.x, y + other.y};
}

Vector2i Vector2i::operator-(const Vector2i& other) const
{
    return {x - other.x, y - other.y};
}

Vector2i Vector2i::operator*(const float multiplier) const
{
    return Vector2i(x * multiplier, y * multiplier);
}

Vector2i Vector2i::operator/(const float divider) const
{
    return Vector2i(x / divider, y / divider);
}