/**
 * @file Intersection.hpp
 */

#pragma once

#include "Bee/Math/Vector2f.hpp"
#include "Bee/World/WorldObject.hpp"

class Entity;

struct Intersection
{
    /**
     * @brief A pointer the colliding entity or NULL if there was no collision with an entity.
     * 
     */
    Entity* entity = nullptr;

    /**
     * @brief A pointer the colliding world object or NULL if there was no collision with a world object.
     * 
     */
    WorldObject* worldObject = nullptr;

    /**
     * @brief The depth of the intersection.
     * 
     */
    float penetrationDepth = 0;

    /**
     * @brief The minimum translation vector to resolve collisions.
     * 
     */
    Vector2f mtv = {0, 0};
};