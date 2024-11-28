#include "Bee/Input/Mouse.hpp"
#include "Mouse-Internal.hpp"

#include <bitset>
#include <unordered_map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Bee/Log.hpp"
#include "Bee/Graphics/Renderer.hpp"

static SDL_Cursor* cursor = nullptr;
static std::unordered_map<uint8_t, MouseButton> mouseMap;
static std::bitset<16> buttonsPressed;
static std::bitset<16> buttonsPressedOld;
static Vector2i mousePositon;

void Mouse::init()
{
    mouseMap.insert({SDL_BUTTON_LEFT, MouseButton::left});
    mouseMap.insert({SDL_BUTTON_MIDDLE, MouseButton::middle});
    mouseMap.insert({SDL_BUTTON_RIGHT, MouseButton::right});
}

void Mouse::handleInput(const SDL_Event* event)
{
    bool buttonPressed = false;

    if (event->type == SDL_MOUSEBUTTONDOWN) buttonPressed = true;

    MouseButton buttonIndex = mouseMap[event->button.button];
    buttonsPressed[static_cast<int>(buttonIndex)] = buttonPressed;
}

void Mouse::handleMovement(const SDL_Event* event)
{
    const Vector2i screenDifference = Renderer::getWindowSize() - Renderer::getScreenSize();
    mousePositon.x = event->motion.x - screenDifference.x / 2;
    mousePositon.y = event->motion.y - screenDifference.y / 2;
}

void Mouse::update()
{
    buttonsPressedOld = buttonsPressed;
}

bool Mouse::isButtonDown(MouseButton button)
{
    return buttonsPressed[static_cast<int>(button)];
}

bool Mouse::isButtonPressed(MouseButton button)
{
    return buttonsPressed[static_cast<int>(button)] && !buttonsPressedOld[static_cast<int>(button)];
}

Vector2i Mouse::getMouseScreenPosition()
{
    return mousePositon;
}

Vector2f Mouse::getMouseWorldPosition()
{
    Vector2f position;
    const Vector2f cameraPosition = Renderer::getCameraPosition();
    const Vector2f viewportSize = Renderer::getViewPortSize();
    const Vector2i screenSize = Renderer::getScreenSize();

    position.x = viewportSize.x * static_cast<float>(mousePositon.x) / screenSize.x - viewportSize.x / 2.0f + cameraPosition.x;
    position.y = viewportSize.y * static_cast<float>(mousePositon.y) / screenSize.y - viewportSize.y / 2.0f + cameraPosition.y;

    return position;
}

void Mouse::createCustomCursor(const std::string& path, const int hotX, const int hotY)
{
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (surface == nullptr)
    {
        Log::write("Input", LogLevel::error, "Can't load image: %s", SDL_GetError());
        return;
    }

    SDL_FreeCursor(cursor);

    cursor = SDL_CreateColorCursor(surface, hotX, hotY);
    if (cursor == nullptr)
    {
        Log::write("Input", LogLevel::warning, "Can't create cursor: %s", SDL_GetError());
    }
    else
    {
        SDL_SetCursor(cursor);
    }

    SDL_FreeSurface(surface);
}

void Mouse::cleanUp()
{
    SDL_FreeCursor(cursor);
}