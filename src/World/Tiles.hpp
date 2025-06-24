#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct AnimationTileFrame
{
    int duration;
    int tileId;
};

struct TileLayer
{
    std::string name;
    std::vector<int> tileIds;
};

struct Tile
{
    bool animated;
    int columns;
    int textureID;
    uint32_t animationIndex;
    uint32_t frameStartTime;
    Vector2i position;
    Vector2i size;
    Vector2i tilesetSize;
    Properties properties;
    std::vector<AnimationTileFrame> animationFrames;
};