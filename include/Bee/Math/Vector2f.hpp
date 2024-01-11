#pragma once

struct Vector2f
{
    float x;
    float y;

    Vector2f();
    Vector2f(float x, float y);
    float dot(const Vector2f& right) const;
    float getLength() const;
    void normalize();
    void operator =(const Vector2f& other);
    void operator +=(const Vector2f& other);
    void operator -=(const Vector2f& other);
    void operator *=(float multiplier);
    void operator *=(const Vector2f& other);
    void operator /=(const Vector2f& other);
    bool operator ==(const Vector2f& other) const;
    Vector2f operator +(const Vector2f& other) const;
    Vector2f operator -(const Vector2f& other) const;
    Vector2f operator *(float multiplier) const;
    Vector2f operator /(float divider) const;
};