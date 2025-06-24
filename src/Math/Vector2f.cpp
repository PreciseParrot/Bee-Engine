#include "Bee/Math/Vector2f.hpp"

#include <cmath>

#include "Bee/Math/Vector2i.hpp"
#include "Bee/Math/Vector3f.hpp"
#include "Bee/Math/Vector4f.hpp"

Vector2f::Vector2f()
    : x(0), y(0) {}

Vector2f::Vector2f(const float x, const float y)
    : x(x), y(y) {}

Vector2f::operator Vector2i() const
{
    return {static_cast<int>(x), static_cast<int>(y)};
}

Vector2f::operator Vector3f() const
{
    return {x, y, 0};
}

Vector2f::operator Vector4f() const
{
    return {x, y, 0, 0};
}

float Vector2f::dot(const Vector2f& other) const
{
    return x * other.x + y * other.y;
}

float Vector2f::getLength() const
{
    return sqrtf(x * x + y * y);
}

void Vector2f::normalize()
{
    const float length = getLength();
    if (length <= 0) return;
    const float scale = 1.0f / length;
    x *= scale;
    y *= scale;
}

Vector2f& Vector2f::operator=(const Vector3f& other)
{
    x = other.x;
    y = other.y;

    return *this;
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

void Vector2f::operator/=(const float divider)
{
    x /= divider;
    y /= divider;
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

Vector2f Vector2f::operator+(const Vector2f& other) const
{
    return {x + other.x, y + other.y};
}

Vector2f Vector2f::operator-() const
{
    return {-x, -y};
}

Vector2f Vector2f::operator-(const Vector2f& other) const
{
    return {x - other.x, y - other.y};
}

Vector2f Vector2f::operator*(const float multiplier) const
{
    return {x * multiplier, y * multiplier};
}

Vector2f Vector2f::operator*(const Vector2f& other) const
{
    return {x * other.x, y * other.y};
}

Vector2f Vector2f::operator/(const float divider) const
{
    return {x / divider, y / divider};
}

Vector2f Vector2f::operator/(const Vector2f& other) const
{
    return {x / other.x, y / other.y};
}
