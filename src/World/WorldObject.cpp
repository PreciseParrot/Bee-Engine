#include "WorldObject.hpp"

Hitbox WorldObject::getHitbox() const
{
    return hitbox;
}

void WorldObject::setHitbox(const Hitbox& hitbox)
{
    this->hitbox = hitbox;
}