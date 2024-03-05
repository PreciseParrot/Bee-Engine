#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>

#include "Graphics/Animation.hpp"
#include "Math/Vector2f.hpp"
#include "Math/Vector2i.hpp"

class Sprite
{
public:
    Sprite();
    void loadSpriteSheet(std::string spriteName);
    void setAnimation(std::string animationName);
    void updateInternalEntity(const Vector2f& position, const Vector2f& scale, const Vector2f& rotationCenter, float rotation);
    void updateInternalHUD(const Vector2i& position, const Vector2i& scale, const Vector2f& rotationCenter, float rotation);
    ~Sprite();

private:
    int currentSprite = 0;
    uint32_t frameStartTime = 0;
    std::string currentAnimationName;
    std::vector<AnimationSpriteFrame> sprites;
    std::unordered_map<std::string, FrameTag> frameTags;
    SDL_Texture* texture = nullptr;
    AnimationDirection currentAnimationDirection = ANIMATION_NONE;
    FrameTag currentAnimation;
    void updateInternal();
};