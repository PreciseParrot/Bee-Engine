#pragma once

enum class AnimationDirection
{
    none,
    forward,
    reverse,
    pingPong,
    reversePingPong,
};

struct FrameTag
{
    int start = 0;
    int end = 0;
    AnimationDirection direction = AnimationDirection::none;
};

struct SpriteFrame
{
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    int duration = 0;
};