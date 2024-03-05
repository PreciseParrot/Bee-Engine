#include "Graphics/HUDObject.hpp"

HUDObject::HUDObject()
{
    sprite = new Sprite;
}

void HUDObject::update() {}

void HUDObject::updateInternal()
{
    sprite->updateInternalHUD(position, scale, rotationCenter, rotation);
}

void HUDObject::setAnimation(std::string animationName)
{
    sprite->setAnimation(animationName);
}

void HUDObject::loadSpriteSheet(std::string spriteName) 
{
    sprite->loadSpriteSheet(spriteName);
}

void HUDObject::setPosition(int x, int y)
{
    position.x = x;
    position.y = y;
}

void HUDObject::setPosition(const Vector2i& position)
{
    this->position = position;
}

void HUDObject::setScale(int width, int height)
{
    scale.x = width;
    scale.y = height;
}

void HUDObject::setScale(const Vector2i& scale)
{
    this->scale = scale;
}

HUDObject::~HUDObject()
{
    delete sprite;
}