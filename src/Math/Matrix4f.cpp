#include "Bee/Math/Matrix4f.hpp"

#include <cmath>
#include <imgui_internal.h>

#include "Bee/Math/Vector3f.hpp"

Matrix4f::Matrix4f()
{
    for (auto& column : elements)
    {
        for (float& element : column)
        {
            element = 0.0f;
        }
    }
}

Matrix4f::Matrix4f(const float s)
{
    for (int i = 0; i < 4; i++)
    {
        elements[i][i] = s;
    }
}

void Matrix4f::rotate(const float angle, const Vector3f& axis)
{
    if (angle == 0.0f) return;

    const float s = sinf(angle);
    const float c = cosf(angle);
    const float omc = 1.0f - c;
    Matrix4f rotation(1.0f);

    rotation.elements[0][0] = c + axis.x * axis.x * omc;
    rotation.elements[0][1] = axis.y * axis.x * omc + axis.z * s;
    rotation.elements[0][2] = axis.z * axis.x * omc - axis.y * s;
    
    rotation.elements[1][0] = axis.x * axis.y * omc - axis.z * s;
    rotation.elements[1][1] = c + axis.y * axis.y * omc;
    rotation.elements[1][2] = axis.z * axis.y * omc + axis.x * s;
    
    rotation.elements[2][0] = axis.x * axis.z * omc + axis.y * s;
    rotation.elements[2][1] = axis.y * axis.z * omc - axis.x * s;
    rotation.elements[2][2] = c + axis.z * axis.z * omc;

    *this = *this * rotation;
}

void Matrix4f::scale(const Vector3f& scale)
{
    elements[0][0] *= scale.x;
    elements[1][1] *= scale.y;
    elements[2][2] *= scale.z;
}

void Matrix4f::translate(const Vector3f& translation)
{
    Matrix4f translationMatrix(1.0f);
    
    translationMatrix.elements[3][0] = translation.x;
    translationMatrix.elements[3][1] = translation.y;
    translationMatrix.elements[3][2] = translation.z;

    *this = *this * translationMatrix;
}

float& Matrix4f::operator()(const int i, const int j)
{
    return elements[i][j];
}

Matrix4f Matrix4f::operator*(const Matrix4f& other) const
{
    Matrix4f result;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                result.elements[i][j] += elements[i][k] * other.elements[k][j];
            }
        }
    }

    return result;
}

Vector4f Matrix4f::operator*(const Vector4f& other) const
{
    Vector4f result;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; ++j)
        {
            result[i] += elements[i][j] * other[i];
        }
    }

    return result;
}