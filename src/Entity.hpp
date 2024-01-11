#pragma once

#include <string>

#include "Audio.hpp"
#include "Input.hpp"
#include "Collision/HitBox.hpp"
#include "Collision/Intersection.hpp"
#include "Graphics/Sprite.hpp"
#include "Math/Vector2f.hpp"

class Entity
{
public:
    Entity();
    void updateInternal();
    HitBox getHitBox() const;
    std::vector<Intersection> getIntersections() const;
    std::string getName() const;
    Vector2f getPosition() const;
    float getRotation() const;
    void loadSpriteSheet(std::string spriteName);
    void moveOffset(const Vector2f& offset);
    void setAnimation(std::string animationName);
    void setName(std::string name);
    void setPosition(float x, float y);
    void setPosition(const Vector2f& position);
    void setRotation(float rotation);
    void setScale(float x, float y);
    void setScale(const Vector2f& scale);
    void setHitboxScale(float x, float y);
    void setHitboxScale(const Vector2f& scale);
    virtual void update();
    virtual ~Entity();

private:
    float rotation = 0; 
    std::string name;
    Sprite* sprite;
    Vector2f position;
    Vector2f rotationCenter = {0.5f, 0.5f};
    Vector2f scale = {1.0f, 1.0f};
    Vector2f hitboxScale = {1.0f, 1.0f};
};