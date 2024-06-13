#pragma once

#include "Collision/Hitbox.hpp"
#include "Collision/Intersection.hpp"

namespace Collision
{
    bool checkCollision(const Hitbox& hitbox1, const Hitbox& hitbox2, Intersection& intersection);
}
