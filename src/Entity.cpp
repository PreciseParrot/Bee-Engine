#include "Entity.hpp"

#include <cmath>
#include <vector>

#include "Audio.hpp"
#include "Bee.hpp"
#include "Math/Vector2f.hpp"

Entity::Entity()
{
    sprite = new Sprite;
}

void Entity::update()
{

}

void Entity::updateInternal()
{
    sprite->updateInternalEntity(position, scale, rotationCenter, rotation);
}

HitBox Entity::getHitBox() const
{
    HitBox hitBox;

    hitBox.hitBoxPoints.push_back(Vector2f(position.x - (hitboxScale.x / 2 * cosf(rotation * M_PI / 180) - (hitboxScale.y / 2 * sinf(rotation * M_PI / 180))), position.y - (hitboxScale.x / 2 * sinf(rotation * M_PI / 180) + (hitboxScale.y / 2 * cosf(rotation * M_PI / 180)))));
    hitBox.hitBoxPoints.push_back(Vector2f(position.x + (hitboxScale.x / 2 * cosf(rotation * M_PI / 180) - (hitboxScale.y / 2 * sinf(rotation * M_PI / 180))), position.y + (hitboxScale.x / 2 * sinf(rotation * M_PI / 180) + (hitboxScale.y / 2 * cosf(rotation * M_PI / 180)))));
    hitBox.hitBoxPoints.push_back(Vector2f(position.x - (hitboxScale.x / 2 * cosf(rotation * M_PI / 180) + (hitboxScale.y / 2 * sinf(rotation * M_PI / 180))), position.y - (hitboxScale.x / 2 * sinf(rotation * M_PI / 180) - (hitboxScale.y / 2 * cosf(rotation * M_PI / 180)))));
    hitBox.hitBoxPoints.push_back(Vector2f(position.x + (hitboxScale.x / 2 * cosf(rotation * M_PI / 180) + (hitboxScale.y / 2 * sinf(rotation * M_PI / 180))), position.y + (hitboxScale.x / 2 * sinf(rotation * M_PI / 180) - (hitboxScale.y / 2 * cosf(rotation * M_PI / 180)))));
    return hitBox;
}

std::vector<Intersection> Entity::getIntersections() const
{
    return Bee::getCurrentWorld()->getIntersections(this);
}

std::string Entity::getName() const
{
    return std::string(name);
}

Vector2f Entity::getPosition() const
{
    return Vector2f(position);
}

float Entity::getRotation() const
{
    return rotation;
}

void Entity::loadSpriteSheet(std::string spriteName)
{
    sprite->loadSpriteSheet(spriteName);
    setScale(1);
    setHitboxScale(1);
}

void Entity::moveOffset(const Vector2f& offset)
{
    position += offset;
}

void Entity::setName(std::string name)
{
    this->name = name;
}

void Entity::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;
}

void Entity::setPosition(const Vector2f& position)
{
    this->position = position;
}

void Entity::setRotation(float rotation)
{
    this->rotation = rotation;
}

void Entity::setScale(float scale)
{
    Vector2i textureSize = sprite->getTextureSize();
    this->scale.x = (float)textureSize.x / textureSize.y * scale;
    this->scale.y = scale;
}

void Entity::setScale(float x, float y)
{
    scale.x = x;
    scale.y = y;
}

void Entity::setScale(const Vector2f& scale)
{
    this->scale = scale;
}

void Entity::setFont(std::string fontName, int size)
{
    sprite->setFont(fontName, size);
}

void Entity::setText(std::string text, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    sprite->setText(text, red, green, blue, alpha);
    setScale(1);
    setHitboxScale(1);
}

void Entity::setHitboxScale(float scale)
{
    Vector2i textureSize = sprite->getTextureSize();
    this->hitboxScale.x = (float)textureSize.x / textureSize.y * scale;
    this->hitboxScale.y = scale;
}

void Entity::setHitboxScale(float x, float y)
{
    hitboxScale.x = x;
    hitboxScale.y = y;
}

void Entity::setHitboxScale(const Vector2f& scale)
{
    hitboxScale = scale;
}

void Entity::setAnimation(std::string animationName)
{
    sprite->setAnimation(animationName);
}

Entity::~Entity()
{
    delete sprite;
}