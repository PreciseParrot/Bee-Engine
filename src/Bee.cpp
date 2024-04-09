#include "Bee.hpp"

#include <SDL2/SDL.h>

#include "Audio.hpp"
#include "Log.hpp"
#include "Graphics/Renderer.hpp"
#include "Input/Controller.hpp"
#include "Input/Keyboard.hpp"
#include "Input/Mouse.hpp"
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
        Log::write("Engine", LogLevel::Error, "Error initializing SDL2: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    Log::write("Engine", LogLevel::Info, "Initialized SDL2");

    currentWorld = nullptr;
    nextWorld = nullptr;

    Audio::init();
    Controller::init();
    Keyboard::init();
    Renderer::init(windowWidth, windowHeight);
}

void Bee::run()
{
    if (!nextWorld)
    {
        Log::write("Engine", LogLevel::Error, "No world loaded");
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
                    Keyboard::handleInput(&event);
                    break;
                case SDL_KEYUP:
                    Keyboard::handleInput(&event);
                    break;
                case SDL_CONTROLLERBUTTONDOWN:
                    Controller::handleInput(&event);
                    break;
                case SDL_CONTROLLERBUTTONUP:
                    Controller::handleInput(&event);
                    break;
                case SDL_CONTROLLERDEVICEADDED:
                    Controller::connectController(&event);
                    break;
                case SDL_CONTROLLERDEVICEREMOVED:
                    Controller::disconnectController(&event);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    Mouse::handleInput(&event);
                    break;
                case SDL_MOUSEBUTTONUP:
                    Mouse::handleInput(&event);
                    break;
                case SDL_MOUSEWHEEL:
                    Mouse::handleInput(&event);
                    break;
                case SDL_MOUSEMOTION:
                    Mouse::handleInput(&event);
                    break;
                case SDL_QUIT:
                    gameRunning = false;
                    break;
            }
        }

        Renderer::update();
        currentWorld->updateInternal();
        currentWorld->update();
        Controller::update();
        Keyboard::update();
        Mouse::update();

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
    Renderer::unloadAllFonts();
    Renderer::cleanUp();
    Controller::cleanUp();
    SDL_Quit();
}