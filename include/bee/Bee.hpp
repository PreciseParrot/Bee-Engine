#pragma once

#include "bee/World/World.hpp"

namespace Bee
{
    void init();
    void run();
    World* getCurrentWorld();
    float getDeltaTime();
    uint32_t getTime();
    void setWorld(World* p_world);
    void cleanUp();
};