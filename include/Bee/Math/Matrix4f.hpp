#pragma once

#include "FWD.hpp"
#include "Vector4f.hpp"

struct Matrix4f
{
    float elements[4][4]{};

    Matrix4f();
    explicit Matrix4f(float s);

    void rotate(float angle, const Vector3f& axis);
    void scale(const Vector3f& scale);
    void translate(const Vector3f& translation);

    float& operator()(int i, int j);

    Matrix4f operator*(const Matrix4f& other) const;
    Vector4f operator*(const Vector4f& other) const;
};
