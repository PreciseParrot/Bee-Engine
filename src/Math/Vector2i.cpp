#include "Vector2i.hpp"

Vector2i::Vector2i()
    : x(0), y(0) {}

Vector2i::Vector2i(int x, int y)
    : x(x), y(y) {}

void Vector2i::operator=(const Vector2i& other)
{
    x = other.x;
    y = other.y;
    return;
}

void Vector2i::operator+=(const Vector2i& other)
{
    x += other.x;
    y += other.y;
    return;
}

void Vector2i::operator-=(const Vector2i& other)
{
    x -= other.x;
    y -= other.y;
    return;
}

bool Vector2i::operator==(const Vector2i& other) const
{
    return x == other.x && y == other.y;
}

Vector2i Vector2i::operator+(const Vector2i& other) const
{
    return Vector2i(x + other.x, y + other.y);
}

Vector2i Vector2i::operator-(const Vector2i& other) const
{
    return Vector2i(x - other.x, y - other.y);
}