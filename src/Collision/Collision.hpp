#pragma once

#include "Bee/Collision/Hitbox.hpp"
#include "Bee/Collision/Intersection.hpp"

namespace Collision
{
    bool checkCollision(const Hitbox& hitbox1, const Hitbox& hitbox2, Intersection& intersection);
}
