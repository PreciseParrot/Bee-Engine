#include "Bee.hpp"

#include <SDL2/SDL.h>

#include "Audio.hpp"
#include "Log.hpp"
#include "Graphics/Renderer.hpp"
#include "Input/Controller.hpp"
#include "Input/Keyboard.hpp"
#include "Input/Mouse.hpp"
#include "World/World.hpp"

static void (*initFunc)() = nullptr;
static bool initialized = false;
static bool gameRunning = false;
static float deltaTime = 0;
static uint32_t currentTime = 0;
static Uint64 loopTicks = 0;
static Uint64 loopTicksLast = 0;
static World* nextWorld = nullptr;
static World* currentWorld = nullptr;

void Bee::init(const int windowWidth, const int windowHeight)
{
    atexit(cleanUp);

    if (SDL_Init(0) < 0)
    {
        Log::write("Engine", LogLevel::error, "Error initializing SDL2: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    Log::write("Engine", LogLevel::info, "Initialized SDL2");

    Renderer::init(windowWidth, windowHeight);
    Audio::init();
    Controller::init();
    Keyboard::init();
    Mouse::init();

    SDL_Event event;
    event.window.event = SDL_WINDOWEVENT_RESIZED;
    event.window.data1 = windowWidth;
    event.window.data2 = windowHeight;
    Renderer::handleEvent(&event);

    if (initFunc) (*initFunc)();

    initialized = true;
}

void Bee::onInit(void (*func)())
{
    initFunc = func;
}

static void mainLoop()
{
    loopTicksLast = loopTicks;
    loopTicks = SDL_GetPerformanceCounter();
    currentTime = SDL_GetTicks();

    if (nextWorld)
    {
        currentWorld->onUnload();
        currentWorld = nextWorld;
        nextWorld = nullptr;
        currentWorld->onLoad();
    }

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_KEYDOWN: case SDL_KEYUP:
                Keyboard::handleInput(&event);
                break;
            case SDL_CONTROLLERBUTTONDOWN: case SDL_CONTROLLERBUTTONUP:
                Controller::handleButtonInput(&event);
                break;
            case SDL_CONTROLLERDEVICEADDED:
                Controller::connectController(&event);
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                Controller::disconnectController(&event);
                break;
            case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
                Mouse::handleInput(&event);
                break;
            case SDL_MOUSEMOTION:
                Mouse::handleMovement(&event);
                break;
            case SDL_WINDOWEVENT:
                Renderer::handleEvent(&event);
                break;
            case SDL_QUIT:
                gameRunning = false;
                break;
            default:
                break;
        }
    }

    Renderer::update();
    currentWorld->update();
    currentWorld->updateInternal();
    Controller::update();
    Keyboard::update();
    Mouse::update();

    deltaTime = static_cast<float>(loopTicks - loopTicksLast) / SDL_GetPerformanceFrequency();
}

void Bee::run()
{
    if (!initialized)
    {
        init(1280, 720);
    }

    if (!nextWorld)
    {
        Log::write("Engine", LogLevel::error, "No world loaded");
        return;
    }

    currentWorld = nextWorld;
    nextWorld = nullptr;
    gameRunning = true;
    currentWorld->onLoad();

    while (gameRunning)
    {
        mainLoop();
    }
    currentWorld->onUnload();
}

void Bee::stop()
{
    gameRunning = false;
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
    Controller::cleanUp();
    Mouse::cleanUp();
    SDL_Quit();
}