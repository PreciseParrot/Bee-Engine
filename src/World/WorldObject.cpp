#include "WorldObject.hpp"

std::string WorldObject::getData(const std::string& index) const
{
    if (data.find(index) == data.end())
        return "";

    return data.at(index);
}

Hitbox WorldObject::getHitbox() const
{
    return hitbox;
}

void WorldObject::setData(const std::string& index, const std::string& data)
{
    this->data.insert({index, data});
}

void WorldObject::setHitbox(const Hitbox& hitbox)
{
    this->hitbox = hitbox;
}