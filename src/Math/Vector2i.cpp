#include "Bee/Math/Vector2i.hpp"

#include <cmath>

#include "Bee/Math/Vector2f.hpp"
#include "Bee/Math/Vector3i.hpp"
#include "Bee/Math/Vector4i.hpp"

Vector2i::Vector2i()
    : x(0), y(0) {}

Vector2i::Vector2i(const int x, const int y)
    : x(x), y(y) {}

Vector2i::operator Vector2f() const
{
    return {static_cast<float>(x), static_cast<float>(y)};
}

Vector2i::operator Vector3i() const
{
    return {x, y, 0};
}

Vector2i::operator Vector4i() const
{
    return {x, y, 0, 0};
}

float Vector2i::getLength() const
{
    return sqrtf(powf(x, 2) + powf(y, 2));
}

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

void Vector2i::operator*=(const float multiplier)
{
    x *= multiplier;
    y *= multiplier;
}

void Vector2i::operator*=(const Vector2i& other)
{
    x *= other.x;
    y *= other.y;
}

void Vector2i::operator/=(float divider)
{
    x /= divider;
    y /= divider;
}

void Vector2i::operator/=(const Vector2i& other)
{
    x /= other.x;
    y /= other.y;
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
    return {static_cast<int>(x * multiplier), static_cast<int>(y * multiplier)};
}

Vector2i Vector2i::operator*(const Vector2i& other) const
{
    return {x * other.x, y * other.y};
}

Vector2i Vector2i::operator/(const float divider) const
{
    return {static_cast<int>(x / divider), static_cast<int>(y / divider)};
}

Vector2i Vector2i::operator/(const Vector2i& other) const
{
    return {x / other.x, y / other.y};
}
