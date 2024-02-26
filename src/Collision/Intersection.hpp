#pragma once

#include "Math/Vector2f.hpp"
#include "World/WorldObject.hpp"

class Entity;

struct Intersection
{
    Entity* entity;
    WorldObject* worldObject;
    Vector2f mtv;
};