#include "Bee/Math/Vector3f.hpp"

#include <cmath>

#include "Bee/Math/Vector2f.hpp"
#include "Bee/Math/Vector2i.hpp"
#include "Bee/Math/Vector3i.hpp"

Vector3f::Vector3f()
{
    x = 0;
    y = 0;
    z = 0;
}

Vector3f::Vector3f(const float x, const float y, const float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3f::operator Vector2f() const
{
    return {x, y};
}

Vector3f::operator Vector2i() const
{
    return {static_cast<int>(x), static_cast<int>(y)};
}

float Vector3f::dot(const Vector3f& other) const
{
    return x * other.x + y * other.y + z * other.z;
}

float Vector3f::getLength() const
{
    return sqrtf(x * x + y * y + z * z);
}

void Vector3f::normalize()
{
    const float length = getLength();
    if (length <= 0) return;
    const float scale = 1.0f / length;
    x *= scale;
    y *= scale;
    z *= scale;
}

void Vector3f::operator+=(const Vector3f& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
}

Vector3f Vector3f::operator+(const Vector3f& other) const
{
    return {x + other.x, y + other.y, z + other.z};
}

Vector3f::operator Vector3i() const
{
    return {static_cast<int>(x), static_cast<int>(y), static_cast<int>(z)};
}
