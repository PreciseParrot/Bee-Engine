#pragma once

#include "World/World.hpp"

namespace Bee
{
    void init(int windowWidth, int windowHeight);
    void run();
    World* getCurrentWorld();
    float getDeltaTime();
    uint32_t getTime();
    void setWorld(World* world);
    void cleanUp();
};