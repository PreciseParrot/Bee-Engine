#include "Bee/Entity.hpp"

#include "Bee/Bee.hpp"
#include "Collision/Collision.hpp"
#include "Graphics/Rect.hpp"
#include "Graphics/Renderer-Internal.hpp"

bool Entity::isCursorOnMe() const
{
    Hitbox hitbox;
    Intersection intersection;

    hitbox.vertices.push_back(Mouse::getMouseWorldPosition());

    return Collision::checkCollision(getHitBox(), hitbox, intersection);
}

std::vector<Intersection> Entity::getIntersections() const
{
    return Bee::getCurrentWorld()->getIntersections(this);
}

void Entity::setScale(const float scale)
{
    const Vector2f textureSize = getTextureSize();
    this->scale.x = textureSize.x / textureSize.y * scale;
    this->scale.y = scale;
}

void Entity::setSprite(const std::string& spriteName)
{
    BaseObject::setSprite(spriteName);
    setScale(1.0f);
}

void Entity::setText(const std::string& text, const std::string& font, int fontSize, const Color& color)
{
    BaseObject::setText(text, font, fontSize, color);
    setScale(1.0f);
}

void Entity::update()
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

    Renderer::queueEntity(position, scale, shaderID, textureID, rect, this);
}
