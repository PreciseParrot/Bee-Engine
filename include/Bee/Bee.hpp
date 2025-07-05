/**
 * @file Bee.hpp
 */

#pragma once

#include <cstdint>

#include "Audio.hpp"
#include "Log.hpp"
#include "Collision/Hitbox.hpp"
#include "Collision/Intersection.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Window.hpp"
#include "Input/Controller.hpp"
#include "Input/Keyboard.hpp"
#include "Input/Mouse.hpp"
#include "Math/Matrix4f.hpp"
#include "Math/Vector2f.hpp"
#include "Math/Vector2i.hpp"
#include "Math/Vector4f.hpp"
#include "World/World.hpp"
#include "World/WorldObject.hpp"

/**
 * @namespace Bee
 * 
 * @brief All the engine related functions.
 * 
 */
namespace Bee
{
    void init(int windowWidth, int windowHeight);
    void onInit(void (*func)());
    void run();
    void cleanUp();
    void stop();

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
     * @return the time in milliseconds since the engine was initialized.
     */
    uint32_t getTime();
};