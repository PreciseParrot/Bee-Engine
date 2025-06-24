#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>

#include "Graphics/Animation.hpp"
#include "Bee/Entity.hpp"
#include "Bee/Graphics/Color.hpp"
#include "Bee/Graphics/HUDObject.hpp"
#include "Bee/Math/Vector2f.hpp"
#include "Bee/Math/Vector2i.hpp"

class Sprite
{
public:
    Sprite();
    void setShader(const std::string& shader);
    void setSprite(const std::string& spriteName);
    void setAnimation(const std::string& animationName);
    void setText(const std::string& text, const std::string& font, int fontSize, const Color& color);
    Vector2f getTextureSize() const;
    void updateInternalEntity(const Vector3f& position, const Vector2f& scale, const Vector2f& rotationCenter, float rotation, Entity* entity);
    void updateInternalHUD(const Vector3f& position, const Vector2i& scale, const Vector2f& rotationCenter, float rotation, HUDObject* hudObject);
    ~Sprite();

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
    void(*onDrawLambda)(void*) = nullptr;

    void updateInternal();
};