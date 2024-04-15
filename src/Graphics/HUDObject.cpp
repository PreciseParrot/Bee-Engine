#include "Graphics/HUDObject.hpp"

#include "Collision/Collision.hpp"
#include "Graphics/Sprite.hpp"
#include "Input/Mouse.hpp"

HUDObject::HUDObject()
{
    sprite = new Sprite;
}

void HUDObject::update() {}

void HUDObject::updateInternal()
{
    sprite->updateInternalHUD(position, scale, rotationCenter, rotation);
}

void HUDObject::setAnimation(const std::string& animationName)
{
    sprite->setAnimation(animationName);
}

void HUDObject::setSprite(const std::string& spriteName) 
{
    sprite->loadSpriteSheet(spriteName);
    setScale(1);
}

Vector2i HUDObject::getSize() const
{
    return sprite->getTextureSize();
}

bool HUDObject::isCursorOnMe() const
{
    HitBox hitBox;
    HitBox cursor;
    Intersection intersection;
    Vector2i center = position + scale / 2;
    Vector2i cursorPosition = Mouse::getMouseScreenPosition();

    hitBox.hitBoxPoints.push_back(Vector2f(center.x - (scale.x / 2 * cosf(rotation * M_PI / 180) - (scale.y / 2 * sinf(rotation * M_PI / 180))), center.y - (scale.x / 2 * sinf(rotation * M_PI / 180) + (scale.y / 2 * cosf(rotation * M_PI / 180)))));
    hitBox.hitBoxPoints.push_back(Vector2f(center.x + (scale.x / 2 * cosf(rotation * M_PI / 180) - (scale.y / 2 * sinf(rotation * M_PI / 180))), center.y + (scale.x / 2 * sinf(rotation * M_PI / 180) + (scale.y / 2 * cosf(rotation * M_PI / 180)))));
    hitBox.hitBoxPoints.push_back(Vector2f(center.x - (scale.x / 2 * cosf(rotation * M_PI / 180) + (scale.y / 2 * sinf(rotation * M_PI / 180))), center.y - (scale.x / 2 * sinf(rotation * M_PI / 180) - (scale.y / 2 * cosf(rotation * M_PI / 180)))));
    hitBox.hitBoxPoints.push_back(Vector2f(center.x + (scale.x / 2 * cosf(rotation * M_PI / 180) + (scale.y / 2 * sinf(rotation * M_PI / 180))), center.y + (scale.x / 2 * sinf(rotation * M_PI / 180) - (scale.y / 2 * cosf(rotation * M_PI / 180)))));

    cursor.hitBoxPoints.push_back(Vector2f(cursorPosition.x, cursorPosition.y));
    
    return Collision::checkCollision(hitBox, cursor, &intersection);
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

void HUDObject::setFont(const std::string& fontName, int size)
{
    sprite->setFont(fontName, size);
}

void HUDObject::setText(const std::string& text, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    sprite->setText(text, red, green, blue, alpha);
    setScale(1);
}

HUDObject::~HUDObject()
{
    delete sprite;
}