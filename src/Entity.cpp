#include "Bee/Entity.hpp"

#include <numbers>
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
    sprite->updateInternalEntity(position, scale, rotationCenter, rotation, this);
}

void Entity::onDraw() {}

Hitbox Entity::getHitBox() const
{
    Hitbox hitbox;
    const float c = cosf(rotation * std::numbers::pi_v<float> / 180.0f);
    const float s = sinf(rotation * std::numbers::pi_v<float> / 180.0f);
    
    hitbox.center = position;
    hitbox.vertices.emplace_back(position.x - hitboxScale.x / 2.0f * c - hitboxScale.y / 2.0f * s, position.y - hitboxScale.x / 2.0f * s + hitboxScale.y / 2.0f * c);
    hitbox.vertices.emplace_back(position.x - hitboxScale.x / 2.0f * c + hitboxScale.y / 2.0f * s, position.y - hitboxScale.x / 2.0f * s - hitboxScale.y / 2.0f * c);
    hitbox.vertices.emplace_back(position.x + hitboxScale.x / 2.0f * c - hitboxScale.y / 2.0f * s, position.y + hitboxScale.x / 2.0f * s + hitboxScale.y / 2.0f * c);
    hitbox.vertices.emplace_back(position.x + hitboxScale.x / 2.0f * c + hitboxScale.y / 2.0f * s, position.y + hitboxScale.x / 2.0f * s - hitboxScale.y / 2.0f * c);

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

Vector3f Entity::getPosition() const
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

void Entity::setShader(const std::string& shader) const
{
    sprite->setShader(shader);
}

void Entity::moveOffset(const Vector2f& offset)
{
    position.x += offset.x;
    position.y += offset.y;
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

void Entity::setPosition(const Vector2f& position)
{
    this->position = position;
}

void Entity::setPosition(const Vector3f& position)
{
    this->position = position;
}

void Entity::setPositionZ(const float z)
{
    position.z = z;
}

void Entity::setRotation(const float rotation)
{
    this->rotation = rotation;
}

void Entity::setScale(const float scale)
{
    const Vector2f textureSize = sprite->getTextureSize();
    this->scale.x = textureSize.x / textureSize.y * scale;
    this->scale.y = scale;
}

void Entity::setScale(const Vector2f& size)
{
    scale = size;
}

void Entity::setText(const std::string& text, const std::string& font, const int fontSize, const Color& color)
{
    sprite->setText(text, font, fontSize, color);
    setScale(1);
    setHitboxScale(1);
}

void Entity::setHitboxScale(const float scale)
{
    const Vector2f textureSize = sprite->getTextureSize();
    this->hitboxScale.x = textureSize.x / textureSize.y * scale;
    this->hitboxScale.y = scale;
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