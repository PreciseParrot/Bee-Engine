#pragma once

#include <string>

#include "bee/Collision/HitBox.hpp"

struct WorldObject
{
    std::string name;
    std::string type;
    HitBox hitBox;
};