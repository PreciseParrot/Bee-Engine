#pragma once

#include <string>

#include "Bee/Collision/HitBox.hpp"

struct WorldObject
{
    std::string name;
    std::string type;
    Hitbox hitBox;
};