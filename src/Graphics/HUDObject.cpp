#include "Bee/Graphics/HUDObject.hpp"

#include "Bee/Collision/Hitbox.hpp"
#include "Bee/Collision/Intersection.hpp"
#include "Bee/Input/Mouse.hpp"
#include "Bee/Math/Vector3f.hpp"
#include "Graphics/Sprite.hpp"
#include "Collision/Collision.hpp"

HUDObject::HUDObject()
{
    sprite = new Sprite;
    position.z = 99;
}

Vector3f HUDObject::getPosition() const
{
    return position;
}

void HUDObject::update()
{
    sprite->updateInternalHUD(position, scale, rotationCenter, rotation, this);
}

void HUDObject::onDraw() {}

void HUDObject::setAnimation(const std::string& animationName) const
{
    sprite->setAnimation(animationName);
}

void HUDObject::setSprite(const std::string& spriteName)
{
    sprite->setSprite(spriteName);
    setScale(1);
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
    const Vector2f center = static_cast<Vector2i>(position) + scale / 2;
    const Vector2i cursorPosition = Mouse::getMouseScreenPosition();
    const float c = cosf(rotation * std::numbers::pi_v<float> / 180.0f);
    const float s = sinf(rotation * std::numbers::pi_v<float> / 180.0f);

    hitbox.vertices.emplace_back(center.x - scale.x / 2.0f * c - scale.y / 2.0f * s, center.y - scale.x / 2.0f * s + scale.y / 2.0f * c);
    hitbox.vertices.emplace_back(center.x + scale.x / 2.0f * c - scale.y / 2.0f * s, center.y + scale.x / 2.0f * s + scale.y / 2.0f * c);
    hitbox.vertices.emplace_back(center.x - scale.x / 2.0f * c + scale.y / 2.0f * s, center.y - scale.x / 2.0f * s - scale.y / 2.0f * c);
    hitbox.vertices.emplace_back(center.x + scale.x / 2.0f * c + scale.y / 2.0f * s, center.y + scale.x / 2.0f * s - scale.y / 2.0f * c);

    cursor.vertices.emplace_back(cursorPosition.x, cursorPosition.y);

    return Collision::checkCollision(hitbox, cursor, intersection);
}

void HUDObject::setPosition(const Vector2f& position)
{
    this->position = position;
}

void HUDObject::setPosition(const Vector3f& position)
{
    this->position = position;
}

void HUDObject::setPositionZ(const float z)
{
    position.z = z;
}

void HUDObject::setScale(const float scale)
{
    const Vector2i textureSize = sprite->getTextureSize();
    this->scale = textureSize * scale;
}

void HUDObject::setScale(const Vector2i& scale)
{
    this->scale = scale;
}

void HUDObject::setShader(const std::string& shader) const
{
    sprite->setShader(shader);
}

void HUDObject::setText(const std::string& text, const std::string& font, const int fontSize, const Color& color)
{
    sprite->setText(text, font, fontSize, color);
    setScale(1);
}

HUDObject::~HUDObject()
{
    delete sprite;
}