#pragma once

#include "Collision/HitBox.hpp"
#include "Collision/Intersection.hpp"

namespace Collision
{
    bool checkCollision(const HitBox& hitBox1, const HitBox& hitBox2, Intersection* p_intersection);
}
