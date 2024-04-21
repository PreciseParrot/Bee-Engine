#include "Mouse.hpp"

#include <bitset>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "Graphics/Renderer.hpp"

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

void Mouse::handleInput(SDL_Event* event)
{
    bool buttonPressed = false;

    if (event->type == SDL_MOUSEBUTTONDOWN) buttonPressed = true;

    MouseButton buttonIndex = mouseMap[event->button.button];
    buttonsPressed[static_cast<int>(buttonIndex)] = buttonPressed;
}

void Mouse::handleMovement(SDL_Event* event)
{
    Vector2i screenDifference = Renderer::getWindowSize() - Renderer::getScreenSize();
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
    Vector2f cameraPosition = Renderer::getCameraPosition();
    Vector2f viewportSize = Renderer::getViewPortSize();
    Vector2i screenSize = Renderer::getScreenSize();

    position.x = viewportSize.x * (float)mousePositon.x / screenSize.x - viewportSize.x / 2.0f + cameraPosition.x;
    position.y = viewportSize.y * (float)mousePositon.y / screenSize.y - viewportSize.y / 2.0f + cameraPosition.y;

    return position;
}