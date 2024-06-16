#include "Graphics/HUDObject.hpp"

#include "Collision/Collision.hpp"
#include "Graphics/Sprite.hpp"
#include "Input/Mouse.hpp"

HUDObject::HUDObject()
{
    sprite = new Sprite;
}

void HUDObject::update() {}

void HUDObject::updateInternal() const
{
    sprite->updateInternalHUD(position, scale, rotationCenter, rotation);
}

void HUDObject::setAnimation(const std::string& animationName) const
{
    sprite->setAnimation(animationName);
}

void HUDObject::setSprite(const std::string& spriteName)
{
    sprite->setSprite(spriteName);
    setSize(1);
}

Vector2i HUDObject::getSize() const
{
    return scale;
}

Vector2i HUDObject::getTextureSize() const
{
    return sprite->getTextureSize();
}

bool HUDObject::isCursorOnMe() const
{
    Hitbox hitbox;
    Hitbox cursor;
    Intersection intersection;
    const Vector2i center = position + scale / 2;
    const Vector2i cursorPosition = Mouse::getMouseScreenPosition();

    hitbox.vertices.emplace_back(center.x - scale.x / 2.0f * cosf(rotation * M_PI / 180.0f) - scale.y / 2.0f * sinf(rotation * M_PI / 180.0f), center.y - scale.x / 2.0f * sinf(rotation * M_PI / 180.0f) + scale.y / 2.0f * cosf(rotation * M_PI / 180.0f));
    hitbox.vertices.emplace_back(center.x + scale.x / 2.0f * cosf(rotation * M_PI / 180.0f) - scale.y / 2.0f * sinf(rotation * M_PI / 180.0f), center.y + scale.x / 2.0f * sinf(rotation * M_PI / 180.0f) + scale.y / 2.0f * cosf(rotation * M_PI / 180.0f));
    hitbox.vertices.emplace_back(center.x - scale.x / 2.0f * cosf(rotation * M_PI / 180.0f) + scale.y / 2.0f * sinf(rotation * M_PI / 180.0f), center.y - scale.x / 2.0f * sinf(rotation * M_PI / 180.0f) - scale.y / 2.0f * cosf(rotation * M_PI / 180.0f));
    hitbox.vertices.emplace_back(center.x + scale.x / 2.0f * cosf(rotation * M_PI / 180.0f) + scale.y / 2.0f * sinf(rotation * M_PI / 180.0f), center.y + scale.x / 2.0f * sinf(rotation * M_PI / 180.0f) - scale.y / 2.0f * cosf(rotation * M_PI / 180.0f));

    cursor.vertices.emplace_back(cursorPosition.x, cursorPosition.y);

    return Collision::checkCollision(hitbox, cursor, intersection);
}

void HUDObject::setPosition(const int x, const int y)
{
    position.x = x;
    position.y = y;
}

void HUDObject::setPosition(const Vector2i& position)
{
    this->position = position;
}

void HUDObject::setSize(const float scale)
{
    const Vector2i textureSize = sprite->getTextureSize();
    this->scale = textureSize * scale;
}

void HUDObject::setSize(const int width, const int height)
{
    scale.x = width;
    scale.y = height;
}

void HUDObject::setSize(const Vector2i& scale)
{
    this->scale = scale;
}

void HUDObject::setFont(const std::string& fontName, const int size) const
{
    sprite->setFont(fontName, size);
}

void HUDObject::setText(const std::string& text, const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha)
{
    sprite->setText(text, red, green, blue, alpha);
    setSize(1);
}

HUDObject::~HUDObject()
{
    delete sprite;
}