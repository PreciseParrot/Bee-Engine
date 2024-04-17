#pragma once

#include "Collision/Hitbox.hpp"
#include "Collision/Intersection.hpp"

namespace Collision
{
    bool checkCollision(const Hitbox& hitBox1, const Hitbox& hitBox2, Intersection* intersection);
}
