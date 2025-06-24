#include "Bee/Math/Math.hpp"

#include <cmath>
#include <cstdio>

float Math::radians(float degrees)
{
    return degrees / 180.0f * M_PI;
}

float Math::degrees(float radians)
{
    return radians * 180.0f / M_PI;
}

Matrix4f Math::ortho(const float left, const float right, const float bottom, const float top, const float near, const float far)
{
    Matrix4f result;

    result(0, 0) = 2.0f / (right - left);
    result(1, 1) = 2.0f / (top - bottom);
    result(2, 2) = -2.0f / (far - near);
    result(3, 0) = -((right + left) / (right - left));
    result(3, 1) = -((top + bottom) / (top - bottom));
    result(3, 2) = -((far + near) / (far - near));
    result(3, 3) = 1.0f;
    
    return result;
}

void Math::printMatrix(const void* matrix)
{
    const float* mat = (float*)matrix;
    for (int i = 0; i < 4; i++)
    {;
        for (int j = 0; j < 4; j++)
        {
            printf("%.2f ", mat[i + j * 4]);
        }
        printf("\n");
    }
    printf("\n");
}
