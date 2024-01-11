#pragma once

#include "Math/Vector2f.hpp"
#include "World/WorldObject.hpp"

class Entity;

struct Intersection
{
    Entity* p_entity;
    WorldObject* p_worldObject;
    Vector2f mtv;
};