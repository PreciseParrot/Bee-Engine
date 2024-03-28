#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <SDL2/SDL.h>

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
    uint32_t animationIndex;
    uint32_t frameStartTime;
    SDL_Texture* texture;
    std::unordered_map<std::string, std::string> data;
    std::vector<AnimationTileFrame> animationFrames;
};