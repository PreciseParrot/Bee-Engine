#include "Graphics/HUDObject.hpp"

#include "Graphics/Sprite.hpp"

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
    setScale(1);
}

Vector2i HUDObject::getSize()
{
    return sprite->getTextureSize();
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

void HUDObject::setScale(float scale)
{
    Vector2i textureSize = sprite->getTextureSize();
    this->scale = textureSize * scale;
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

void HUDObject::setFont(std::string fontName, int size)
{
    sprite->setFont(fontName, size);
}

void HUDObject::setText(std::string text, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    sprite->setText(text, red, green, blue, alpha);
    setScale(1);
}

HUDObject::~HUDObject()
{
    delete sprite;
}