#pragma once

#include "Bee/Math/Vector2f.hpp"
#include "Bee/World/WorldObject.hpp"

class Entity;

struct Intersection
{
    Entity* entity;
    WorldObject* worldObject;
    Vector2f mtv;
};