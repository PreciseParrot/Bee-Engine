#pragma once

#include "Matrix4f.hpp"

namespace Math
{
    float radians(float degrees);

    float degrees(float radians);
    
    Matrix4f ortho(float left, float right, float bottom, float top, float near, float far);

    void printMatrix(const void* matrix);
}