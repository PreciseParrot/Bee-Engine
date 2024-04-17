#pragma once

#include <string>

#include "Collision/Hitbox.hpp"

struct WorldObject
{
    std::string name;
    std::string type;
    Hitbox hitBox;
};