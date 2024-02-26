#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct SDL_Texture;

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
    std::string type;
    bool animated;
    int currentX, currentY;
    int x, y;
    int width, height;
    int columns;
    int tilesetWidth, tilesetHeight;
    int animationIndex;
    uint32_t frameStartTime;
    SDL_Texture* texture;
    std::vector<AnimationTileFrame> animationFrames;
};