#pragma once

#include <string>

#include "bee/Audio.hpp"
#include "bee/Input.hpp"
#include "bee/Collision/HitBox.hpp"
#include "bee/Collision/Intersection.hpp"
#include "bee/Math/Vector2f.hpp"

class Sprite;

class Entity
{
public:
    Entity();
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