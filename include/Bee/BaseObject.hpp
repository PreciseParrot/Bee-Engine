#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "Bee/Collision/Hitbox.hpp"
#include "Bee/Graphics/Animation.hpp"
#include "Bee/Graphics/Color.hpp"
#include "Bee/Math/Vector2f.hpp"
#include "Bee/Math/Vector3f.hpp"

class Entity;
class HUDObject;

class BaseObject
{
    friend Entity;
    friend HUDObject;
    
public:
    void setShader(const std::string& shader);
    void setAnimation(const std::string& animationName);
    void setSprite(const std::string& spriteName);
    void setText(const std::string& text, const std::string& font, int fontSize, const Color& color);
    void setScale(const Vector2f& scale);
    void setHitboxScale(float scale);
    void setHitboxScale(const Vector2f& scale);
    void setPosition(const Vector2f& position);
    void setPosition(const Vector3f& position);
    void setPositionZ(float z);
    void moveOffset(const Vector2f& offset);
    Vector3f getPosition() const;
    Vector2f getScale() const;
    Vector2i getTextureSize() const;
    Hitbox getHitBox() const;
    virtual void update();
    virtual void onDraw();
    virtual ~BaseObject() = default;

private:
    int shaderID = 0;
    int textureID = 0;
    int currentSprite = 0;
    uint32_t frameStartTime = 0;
    std::string currentAnimationName;
    std::vector<SpriteFrame> frames;
    std::unordered_map<std::string, FrameTag> animations;
    AnimationDirection currentAnimationDirection = AnimationDirection::none;
    FrameTag currentAnimation = {};
    Vector3f position;
    Vector2f scale = {1.0f, 1.0f};
    Vector2f rotationCenter = {0.5f, 0.5f};
    Vector2f hitboxScale = {1.0f, 1.0f};
};
