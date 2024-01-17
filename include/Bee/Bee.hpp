#pragma once

#include <string>

#include "Bee/World/World.hpp"

namespace Bee
{
    void init(int windowWidth, int windowHeight);
    void run();
    World* getCurrentWorld();
    float getDeltaTime();
    uint32_t getTime();
    void setWorld(World* p_world);
    void cleanUp();
};