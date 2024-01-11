#include "Bee.hpp"

#include <algorithm>
#include <iostream>
#include <SDL2/SDL.h>

#include "Audio.hpp"
#include "Graphics/Renderer.hpp"
#include "World/World.hpp"

static bool gameRunning = true;
static float deltaTime;
static uint32_t currentTime;
static Uint64 loopTicks = 0;
static Uint64 loopTicksLast = 0;
static World* p_nextWorld;
static World* p_currentWorld;

void Bee::init(int windowWidth, int windowHeight)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "Error initializing SDL2: " << SDL_GetError() << std::endl;
        throw std::exception();
    }
    std::cout << "Initialized SDL2" << std::endl;

    p_currentWorld = nullptr;
    p_nextWorld = nullptr;

    Renderer::init(windowWidth, windowHeight);
    Audio::init();
    Input::init();
}

void Bee::run()
{
    if (!p_nextWorld)
    {
        std::cout << "No world loaded" << std::endl;
        return;
    }

    p_currentWorld = p_nextWorld;
    p_nextWorld = nullptr;
    p_currentWorld->onLoad();

    while (gameRunning)
    {
        loopTicksLast = loopTicks;
        loopTicks = SDL_GetPerformanceCounter();

        currentTime = SDL_GetTicks();

        if (p_nextWorld)
        {
            p_currentWorld->onUnLoad();
            p_currentWorld = p_nextWorld;
            p_nextWorld = nullptr;
            p_currentWorld->onLoad();
        }

        SDL_Event gameEvent;
        while (SDL_PollEvent(&gameEvent))
        {
            switch (gameEvent.type)
            {
                case SDL_KEYDOWN:
                    Input::handleInput(&gameEvent);
                    break;
                case SDL_KEYUP:
                    Input::handleInput(&gameEvent);
                    break;
                case SDL_QUIT:
                    gameRunning = false;
                    break;
            }
        }

        Renderer::clear();
        p_currentWorld->updateInternal();
        p_currentWorld->update();
        Renderer::display();
        Input::update();

        deltaTime = (float)(loopTicks - loopTicksLast) / SDL_GetPerformanceFrequency();
    }
}

World* Bee::getCurrentWorld()
{
    return p_currentWorld;
}

float Bee::getDeltaTime()
{
    return deltaTime;
}

uint32_t Bee::getTime()
{
    return currentTime;
}

void Bee::setWorld(World* p_world)
{
    p_nextWorld = p_world;
}

void Bee::cleanUp()
{
    Audio::cleanUp();
    Renderer::cleanUp();
    SDL_Quit();
}