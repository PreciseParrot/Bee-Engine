#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Graphics/Animation.hpp"
#include "Math/Vector2f.hpp"
#include "Math/Vector2i.hpp"

class Sprite
{
public:
    Sprite();
    void setSprite(const std::string& spriteName);
    void setAnimation(const std::string& animationName);
    void setFont(const std::string& fontName, int size);
    void setText(const std::string& text, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
    Vector2i getTextureSize();
    void updateInternalEntity(const Vector2f& position, const Vector2f& scale, const Vector2f& rotationCenter, float rotation);
    void updateInternalHUD(const Vector2i& position, const Vector2i& scale, const Vector2f& rotationCenter, float rotation);
    ~Sprite();

private:
    int currentSprite = 0;
    uint32_t frameStartTime = 0;
    std::string currentAnimationName;
    std::string text;
    std::vector<AnimationSpriteFrame> sprites;
    std::unordered_map<std::string, FrameTag> frameTags;
    SDL_Texture* texture = nullptr;
    TTF_Font* font = nullptr;
    AnimationDirection currentAnimationDirection = ANIMATION_NONE;
    FrameTag currentAnimation;
    void updateInternal();
};