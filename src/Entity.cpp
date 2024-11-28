#include "Bee/Entity.hpp"

#include <cmath>
#include <vector>

#include "Bee/Bee.hpp"
#include "Collision/Collision.hpp"
#include "Graphics/Sprite.hpp"

Entity::Entity()
{
    sprite = new Sprite;
}

void Entity::update()
{
    sprite->updateInternalEntity(position, scale, rotationCenter, rotation);
}

Hitbox Entity::getHitBox() const
{
    Hitbox hitbox;

    hitbox.center = position;
    hitbox.vertices.emplace_back(position.x - hitboxScale.x / 2 * cosf(rotation * M_PI / 180) - hitboxScale.y / 2 * sinf(rotation * M_PI / 180), position.y - hitboxScale.x / 2 * sinf(rotation * M_PI / 180) + hitboxScale.y / 2 * cosf(rotation * M_PI / 180));
    hitbox.vertices.emplace_back(position.x - hitboxScale.x / 2 * cosf(rotation * M_PI / 180) + hitboxScale.y / 2 * sinf(rotation * M_PI / 180), position.y - hitboxScale.x / 2 * sinf(rotation * M_PI / 180) - hitboxScale.y / 2 * cosf(rotation * M_PI / 180));
    hitbox.vertices.emplace_back(position.x + hitboxScale.x / 2 * cosf(rotation * M_PI / 180) - hitboxScale.y / 2 * sinf(rotation * M_PI / 180), position.y + hitboxScale.x / 2 * sinf(rotation * M_PI / 180) + hitboxScale.y / 2 * cosf(rotation * M_PI / 180));
    hitbox.vertices.emplace_back(position.x + hitboxScale.x / 2 * cosf(rotation * M_PI / 180) + hitboxScale.y / 2 * sinf(rotation * M_PI / 180), position.y + hitboxScale.x / 2 * sinf(rotation * M_PI / 180) - hitboxScale.y / 2 * cosf(rotation * M_PI / 180));

    return hitbox;
}

std::vector<Intersection> Entity::getIntersections() const
{
    return Bee::getCurrentWorld()->getIntersections(this);
}

std::string Entity::getName() const
{
    return properties.getString("name");
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

    hitbox.vertices.push_back(Mouse::getMouseWorldPosition());

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

void Entity::setName(const std::string& name)
{
    properties.setString("name", name);
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