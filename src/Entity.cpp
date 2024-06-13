#include "Entity.hpp"

#include <cmath>
#include <vector>

#include "Audio.hpp"
#include "Bee.hpp"
#include "Collision/Collision.hpp"
#include "Input/Mouse.hpp"
#include "Math/Vector2f.hpp"

Entity::Entity()
{
    sprite = new Sprite;
}

void Entity::update()
{

}

void Entity::updateInternal() const
{
    sprite->updateInternalEntity(position, scale, rotationCenter, rotation);
}

Hitbox Entity::getHitBox() const
{
    Hitbox hitbox;

    hitbox.center = position;
    hitbox.hitboxVertices.emplace_back(position.x - hitboxScale.x / 2 * cosf(rotation * M_PI / 180) - hitboxScale.y / 2 * sinf(rotation * M_PI / 180), position.y - hitboxScale.x / 2 * sinf(rotation * M_PI / 180) + hitboxScale.y / 2 * cosf(rotation * M_PI / 180));
    hitbox.hitboxVertices.emplace_back(position.x - hitboxScale.x / 2 * cosf(rotation * M_PI / 180) + hitboxScale.y / 2 * sinf(rotation * M_PI / 180), position.y - hitboxScale.x / 2 * sinf(rotation * M_PI / 180) - hitboxScale.y / 2 * cosf(rotation * M_PI / 180));
    hitbox.hitboxVertices.emplace_back(position.x + hitboxScale.x / 2 * cosf(rotation * M_PI / 180) - hitboxScale.y / 2 * sinf(rotation * M_PI / 180), position.y + hitboxScale.x / 2 * sinf(rotation * M_PI / 180) + hitboxScale.y / 2 * cosf(rotation * M_PI / 180));
    hitbox.hitboxVertices.emplace_back(position.x + hitboxScale.x / 2 * cosf(rotation * M_PI / 180) + hitboxScale.y / 2 * sinf(rotation * M_PI / 180), position.y + hitboxScale.x / 2 * sinf(rotation * M_PI / 180) - hitboxScale.y / 2 * cosf(rotation * M_PI / 180));

    return hitbox;
}

std::vector<Intersection> Entity::getIntersections() const
{
    return Bee::getCurrentWorld()->getIntersections(this);
}

std::string Entity::getName() const
{
    return getData("name");
}

std::string Entity::getData(const std::string& index) const
{
    if (data.find(index) == data.end())
        return "";

    return data.at(index);
}

Vector2f Entity::getPosition() const
{
    return position;
}

float Entity::getRotation() const
{
    return rotation;
}

bool Entity::isCursorOnMe() const
{
    Hitbox hitbox;
    Intersection intersection;

    hitbox.hitboxVertices.push_back(Mouse::getMouseWorldPosition());

    return Collision::checkCollision(getHitBox(), hitbox, intersection);
}

void Entity::moveOffset(const Vector2f& offset)
{
    position += offset;
}

void Entity::setSprite(const std::string& spriteName)
{
    sprite->setSprite(spriteName);
    setScale(1);
    setHitboxScale(1);
}

void Entity::setData(const std::string& index, const std::string& data)
{
    this->data.insert({index, data});
}

void Entity::setName(const std::string& name)
{
    setData("name", name);
}

void Entity::setPosition(const float x, const float y)
{
    position.x = x;
    position.y = y;
}

void Entity::setPosition(const Vector2f& position)
{
    this->position = position;
}

void Entity::setRotation(const float rotation)
{
    this->rotation = rotation;
}

void Entity::setScale(const float scale)
{
    const Vector2i textureSize = sprite->getTextureSize();
    this->scale.x = static_cast<float>(textureSize.x) / textureSize.y * scale;
    this->scale.y = scale;
}

void Entity::setScale(const float width, const float height)
{
    scale.x = width;
    scale.y = height;
}

void Entity::setScale(const Vector2f& size)
{
    scale = size;
}

void Entity::setFont(const std::string& fontName, const int size) const
{
    sprite->setFont(fontName, size);
}

void Entity::setText(const std::string& text, const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha)
{
    sprite->setText(text, red, green, blue, alpha);
    setScale(1);
    setHitboxScale(1);
}

void Entity::setHitboxScale(const float scale)
{
    const Vector2i textureSize = sprite->getTextureSize();
    this->hitboxScale.x = static_cast<float>(textureSize.x) / textureSize.y * scale;
    this->hitboxScale.y = scale;
}

void Entity::setHitboxScale(const float width, const float height)
{
    hitboxScale.x = width;
    hitboxScale.y = height;
}

void Entity::setHitboxScale(const Vector2f& scale)
{
    hitboxScale = scale;
}

void Entity::setAnimation(const std::string& animationName) const
{
    sprite->setAnimation(animationName);
}

Entity::~Entity()
{
    delete sprite;
}