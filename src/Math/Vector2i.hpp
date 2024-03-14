#pragma once

struct Vector2i
{
    int x;
    int y;

    Vector2i();
    Vector2i(int x, int y);
    void operator=(const Vector2i& other);
    void operator+=(const Vector2i& other);
    void operator-=(const Vector2i& other);
    bool operator==(const Vector2i& other) const;
    Vector2i operator+(const Vector2i& other) const;
    Vector2i operator-(const Vector2i& other) const;
    Vector2i operator *(float multiplier) const;
    Vector2i operator /(float divider) const;
};