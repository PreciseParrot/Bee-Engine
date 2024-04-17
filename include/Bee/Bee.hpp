/**
 * @file Bee.hpp
 */

#pragma once

#include <cstdint>

#include "Bee/World/World.hpp"

/**
 * @namespace Bee
 * 
 * @brief All the engine related functions.
 * 
 */
namespace Bee
{
    void init(int windowWidth, int windowHeight);
    void run();
    void cleanUp();

    /**
     * @brief Get the current world.
     * 
     * @return a pointer to the current world. 
     */
    World* getCurrentWorld();

    /**
     * @brief Set the current world.
     * 
     * @param world the pointer to the world
     */
    void setWorld(World* world);

    /**
     * @brief Get the delta time
     * 
     * @return the time in seconds it took for a frame to render. Used to make sure that everything moves at the same speed.
     */
    float getDeltaTime();

    /**
     * @brief Get the time passed since the start of the game
     * 
     * @return the the time in millisconds since the engine was initialized.
     */
    uint32_t getTime();
};