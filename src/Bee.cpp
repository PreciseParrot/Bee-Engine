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
static World* nextWorld;
static World* currentWorld;

void Bee::init(int windowWidth, int windowHeight)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        std::cout << "Error initializing SDL2: " << SDL_GetError() << std::endl;
        throw std::exception();
    }
    std::cout << "Initialized SDL2" << std::endl;

    currentWorld = nullptr;
    nextWorld = nullptr;

    Renderer::init(windowWidth, windowHeight);
    Audio::init();
    Input::init();
}

void Bee::run()
{
    if (!nextWorld)
    {
        std::cout << "No world loaded" << std::endl;
        return;
    }

    currentWorld = nextWorld;
    nextWorld = nullptr;
    currentWorld->onLoad();

    while (gameRunning)
    {
        loopTicksLast = loopTicks;
        loopTicks = SDL_GetPerformanceCounter();

        currentTime = SDL_GetTicks();

        if (nextWorld)
        {
            currentWorld->onUnLoad();
            currentWorld = nextWorld;
            nextWorld = nullptr;
            currentWorld->onLoad();
        }

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_KEYDOWN:
                    Input::handleInput(&event);
                    break;
                case SDL_KEYUP:
                    Input::handleInput(&event);
                    break;
                case SDL_QUIT:
                    gameRunning = false;
                    break;
            }
        }

        Renderer::clear();
        currentWorld->updateInternal();
        currentWorld->update();
        Renderer::display();
        Input::update();

        deltaTime = (float)(loopTicks - loopTicksLast) / SDL_GetPerformanceFrequency();
    }
}

World* Bee::getCurrentWorld()
{
    return currentWorld;
}

float Bee::getDeltaTime()
{
    return deltaTime;
}

uint32_t Bee::getTime()
{
    return currentTime;
}

void Bee::setWorld(World* world)
{
    nextWorld = world;
}

void Bee::cleanUp()
{
    Audio::cleanUp();
    Renderer::cleanUp();
    SDL_Quit();
}