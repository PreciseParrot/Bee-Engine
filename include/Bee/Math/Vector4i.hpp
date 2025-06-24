#pragma once

#include "FWD.hpp"

struct Vector4i
{
    int x;
    int y;
    int z;
    int w;

    Vector4i();
    Vector4i(int x, int y, int z, int w);

    operator Vector4f() const;
};
