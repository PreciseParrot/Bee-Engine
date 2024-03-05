#pragma once

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