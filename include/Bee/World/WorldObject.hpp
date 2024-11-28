/**
* @file WorldObject.hpp
 */

#pragma once

#include "Bee/Properties.hpp"
#include "Bee/Collision/Hitbox.hpp"

class WorldObject
{
public:
    /**
     * @brief The custom properties of the world object.
     *
     */
    Properties properties;

    Hitbox getHitbox() const;
    
    void setHitbox(const Hitbox& hitbox);

private:
    Hitbox hitbox;
};