#pragma once

#include "FWD.hpp"

struct Vector3i
{
    int x;
    int y;
    int z;

    Vector3i();
    Vector3i(int x, int y, int z);

    operator Vector3f() const;
};
