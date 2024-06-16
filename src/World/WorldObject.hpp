/**
 * @file WorldObject.hpp
 */

#pragma once

#include "Properties.hpp"
#include "Collision/Hitbox.hpp"

class WorldObject
{
public:
    //Internal functions start here

    Hitbox getHitbox() const;
    void setHitbox(const Hitbox& hitbox);

    //Internal functions end here

    /**
     * @brief The custom properties of the world object.
     *
     */
    Properties properties;

private:
    Hitbox hitbox;
};