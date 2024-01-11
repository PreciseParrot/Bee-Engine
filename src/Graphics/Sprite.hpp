#pragma once

#include <map>
#include <string>
#include <vector>

#include <SDL2/SDL.h>

#include "Math/Vector2f.hpp"

enum AnimationDirection
{
    ANIMATION_NONE,
    ANIMATION_FORWARD,
    ANIMATION_REVERSE,
    ANIMATION_PINGPONG
};

struct FrameTag
{
    int start;
    int end;
    AnimationDirection direction;
};

struct AnimationSpriteFrame
{
    int x;
    int y;
    int w;
    int h;
    int duration;
};

class Sprite
{
public:
    Sprite();
    void loadSpriteSheet(std::string spriteName);
    void setAnimation(std::string animationName);
    void updateInternal(Vector2f& position, Vector2f& scale, Vector2f& rotationCenter, float rotation);
    ~Sprite();

private:
    int currentSprite = 0;
    uint32_t frameStartTime = 0;
    std::string currentAnimationName;
    std::vector<AnimationSpriteFrame> sprites;
    std::map<std::string, FrameTag> frameTags;
    SDL_Texture* p_texture = nullptr;
    AnimationDirection currentAnimationDirection = ANIMATION_NONE;
    FrameTag currentAnimation;
};