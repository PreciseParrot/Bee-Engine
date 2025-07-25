#include "Bee/Bee.hpp"

#include <imgui_impl_sdl2.h>
#include <SDL2/SDL.h>

#include "Audio-Internal.hpp"
#include "Graphics/Renderer-Internal.hpp"
#include "Input/Controller-Internal.hpp"
#include "Input/Keyboard-Internal.hpp"
#include "Input/Mouse-Internal.hpp"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

static void (*initFunc)() = nullptr;
static bool initialized = false;
static bool gameRunning = false;
static float deltaTime = 0;
static uint32_t currentTime = 0;
static uint64_t loopTicks = 0;
static uint64_t loopTicksLast = 0;
static World* nextWorld = nullptr;
static World* currentWorld = nullptr;

void Bee::init(const int windowWidth, const int windowHeight)
{
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

    if (initFunc) (*initFunc)();

    initialized = true;
}

void Bee::onInit(void (*func)())
{
    initFunc = func;
}

static void mainLoop()
{
    if (!initialized)
    {
        Bee::init(1920, 1080);
        
        if (!currentWorld && !nextWorld)
        {
            Log::write("Engine", LogLevel::error, "No world loaded");
            return;
        }

        currentWorld = nextWorld;
        nextWorld = nullptr;
        gameRunning = true;
        currentWorld->onLoad();
    }
    
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
        ImGui_ImplSDL2_ProcessEvent(&event);

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

    currentWorld->World::update();
    currentWorld->update();
    Renderer::update();
    Controller::update();
    Keyboard::update();
    Mouse::update();

    deltaTime = static_cast<float>(loopTicks - loopTicksLast) / SDL_GetPerformanceFrequency();
}

void Bee::run()
{
    gameRunning = true;

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(mainLoop, 0, true);
#else
    while (gameRunning) mainLoop();
#endif
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
    Renderer::cleanUp();
    Audio::cleanUp();
    Controller::cleanUp();
    Mouse::cleanUp();
    SDL_Quit();
}