#include "Bee/Graphics/HUDObject.hpp"

#include "Bee/Input/Mouse.hpp"
#include "Bee/Math/Vector2i.hpp"
#include "Collision/Collision.hpp"
#include "Graphics/Rect.hpp"
#include "Graphics/Renderer-Internal.hpp"

bool HUDObject::isCursorOnMe() const
{
    Hitbox hitbox;
    Hitbox cursor;
    Intersection intersection;
    const Vector2f center = static_cast<Vector2i>(position) + scale / 2;
    const Vector2i cursorPosition = Mouse::getMouseScreenPosition();
    const float c = cosf(0.0f * std::numbers::pi_v<float> / 180.0f);
    const float s = sinf(0.0f * std::numbers::pi_v<float> / 180.0f);

    hitbox.vertices.emplace_back(center.x - scale.x / 2.0f * c - scale.y / 2.0f * s, center.y - scale.x / 2.0f * s + scale.y / 2.0f * c);
    hitbox.vertices.emplace_back(center.x + scale.x / 2.0f * c - scale.y / 2.0f * s, center.y + scale.x / 2.0f * s + scale.y / 2.0f * c);
    hitbox.vertices.emplace_back(center.x - scale.x / 2.0f * c + scale.y / 2.0f * s, center.y - scale.x / 2.0f * s - scale.y / 2.0f * c);
    hitbox.vertices.emplace_back(center.x + scale.x / 2.0f * c + scale.y / 2.0f * s, center.y + scale.x / 2.0f * s - scale.y / 2.0f * c);

    cursor.vertices.emplace_back(cursorPosition.x, cursorPosition.y);

    return Collision::checkCollision(hitbox, cursor, intersection);
}

void HUDObject::setScale(const float scale)
{
    const Vector2i textureSize = getTextureSize();
    this->scale = textureSize * scale;
}

void HUDObject::setSprite(const std::string& spriteName)
{
    BaseObject::setSprite(spriteName);
    setScale(1.0f);
}

void HUDObject::setText(const std::string& text, const std::string& font, int fontSize, const Color& color)
{
    BaseObject::setText(text, font, fontSize, color);
    setScale(1.0f);
}

void HUDObject::update()
{
    Rect rect;

    if (frames.empty())
    {
        Vector2i textureSize = getTextureSize();
        rect.x = 0;
        rect.y = 0;
        rect.w = textureSize.x;
        rect.h = textureSize.y;
    }
    else
    {
        rect.x = frames.at(currentSprite).x;
        rect.y = frames.at(currentSprite).y;
        rect.w = frames.at(currentSprite).w;
        rect.h = frames.at(currentSprite).h;
    }

    Renderer::queueHUD(position, scale, shaderID, textureID, rect, this);
}
