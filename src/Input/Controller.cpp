#include "Bee/Input/Controller.hpp"
#include "Controller-Internal.hpp"

#include <array>
#include <bitset>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "Bee/Log.hpp"
#include "Bee/Math/Vector2f.hpp"

static std::unordered_map<SDL_GameControllerButton, ControllerButton> controllerMap;
static std::array<std::bitset<32>, 8> buttonsPressed;
static std::array<std::bitset<32>, 8> buttonsPressedOld;
static std::array<SDL_GameController*, 8> controller;
static Vector2f controllerDeadzone(0.25f, 0.25f);

void Controller::init()
{
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) < 0)
    {
        Log::write("Input", LogLevel::error, "Error initializing controller subsystem: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    controllerMap.insert({SDL_CONTROLLER_BUTTON_INVALID, ControllerButton::unknown});
    controllerMap.insert({SDL_CONTROLLER_BUTTON_A, ControllerButton::a});
    controllerMap.insert({SDL_CONTROLLER_BUTTON_B, ControllerButton::b});
    controllerMap.insert({SDL_CONTROLLER_BUTTON_X, ControllerButton::x});
    controllerMap.insert({SDL_CONTROLLER_BUTTON_Y, ControllerButton::y});
    controllerMap.insert({SDL_CONTROLLER_BUTTON_DPAD_LEFT, ControllerButton::left});
    controllerMap.insert({SDL_CONTROLLER_BUTTON_DPAD_RIGHT, ControllerButton::right});
    controllerMap.insert({SDL_CONTROLLER_BUTTON_DPAD_UP, ControllerButton::up});
    controllerMap.insert({SDL_CONTROLLER_BUTTON_DPAD_DOWN, ControllerButton::down});
    controllerMap.insert({SDL_CONTROLLER_BUTTON_BACK, ControllerButton::select});
    controllerMap.insert({SDL_CONTROLLER_BUTTON_START, ControllerButton::start});
    controllerMap.insert({SDL_CONTROLLER_BUTTON_LEFTSTICK, ControllerButton::leftStick});
    controllerMap.insert({SDL_CONTROLLER_BUTTON_RIGHTSTICK, ControllerButton::rightStick});
    controllerMap.insert({SDL_CONTROLLER_BUTTON_LEFTSHOULDER, ControllerButton::leftShoulder});
    controllerMap.insert({SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, ControllerButton::rightShoulder});

    Log::write("Input", LogLevel::info, "Initialized input");
}

bool Controller::isButtonDown(const int playerIndex, ControllerButton button)
{
    if (!isControllerConnected(playerIndex)) return false;
    
    return buttonsPressed[SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(SDL_GameControllerFromPlayerIndex(playerIndex)))][static_cast<int>(button)];
}

bool Controller::isButtonPressed(const int playerIndex, ControllerButton button)
{
    if (!isControllerConnected(playerIndex)) return false;
    
    return buttonsPressed[SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(SDL_GameControllerFromPlayerIndex(playerIndex)))][static_cast<int>(button)] && !buttonsPressed[SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(SDL_GameControllerFromPlayerIndex(playerIndex)))][static_cast<int>(button)];
}

Vector2f Controller::getLeftStick(const int playerIndex)
{
    Vector2f stick;

    if (!isControllerConnected(playerIndex)) return stick;

    stick.x = SDL_GameControllerGetAxis(SDL_GameControllerFromPlayerIndex(playerIndex), SDL_CONTROLLER_AXIS_LEFTX) / 32768.0f;
    stick.y = SDL_GameControllerGetAxis(SDL_GameControllerFromPlayerIndex(playerIndex), SDL_CONTROLLER_AXIS_LEFTY) / 32768.0f;

    if (stick.x < controllerDeadzone.x && stick.x > -controllerDeadzone.x) stick.x = 0;
    if (stick.y < controllerDeadzone.y && stick.y > -controllerDeadzone.y) stick.y = 0;

    return stick;
}

Vector2f Controller::getRightStick(const int playerIndex)
{
    Vector2f stick;

    if (!isControllerConnected(playerIndex)) return stick;

    stick.x = SDL_GameControllerGetAxis(SDL_GameControllerFromPlayerIndex(playerIndex), SDL_CONTROLLER_AXIS_RIGHTX) / 32768.0f;
    stick.y = SDL_GameControllerGetAxis(SDL_GameControllerFromPlayerIndex(playerIndex), SDL_CONTROLLER_AXIS_RIGHTY) / 32768.0f;

    if (stick.x < controllerDeadzone.x && stick.x > -controllerDeadzone.x) stick.x = 0;
    if (stick.y < controllerDeadzone.y && stick.y > -controllerDeadzone.y) stick.y = 0;

    return stick;
}

float Controller::getLeftTrigger(const int playerIndex)
{
    if (!isControllerConnected(playerIndex)) return 0;
    
    const float trigger = SDL_GameControllerGetAxis(SDL_GameControllerFromPlayerIndex(playerIndex), SDL_CONTROLLER_AXIS_TRIGGERLEFT) / 32768.0f;
    return trigger > controllerDeadzone.x ? trigger : 0;
}

float Controller::getRightTrigger(const int playerIndex)
{
    if (!isControllerConnected(playerIndex)) return 0;
    
    const float trigger = SDL_GameControllerGetAxis(SDL_GameControllerFromPlayerIndex(playerIndex), SDL_CONTROLLER_AXIS_TRIGGERRIGHT) / 32768.0f;
    return trigger > controllerDeadzone.y ? trigger : 0;
}

bool Controller::isControllerConnected(int playerIndex)
{
    return controller.at(playerIndex);
}

void Controller::update()
{
    for (size_t i = 0; i < buttonsPressed.size(); i++)
    {
        buttonsPressedOld[i] = buttonsPressed[i];

        if (SDL_GameControllerGetAxis(controller[i], SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 100 && controller[i])
        {
            buttonsPressed[i][static_cast<int>(ControllerButton::leftTrigger)] = true;
        }
        else
        {
            buttonsPressed[i][static_cast<int>(ControllerButton::leftTrigger)] = false;
        }

        if (SDL_GameControllerGetAxis(controller[i], SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 100 && controller[i])
        {
            buttonsPressed[i][static_cast<int>(ControllerButton::rightTrigger)] = true;
        }
        else
        {
            buttonsPressed[i][static_cast<int>(ControllerButton::rightTrigger)] = false;
        }
    }
}

void Controller::handleButtonInput(const SDL_Event* event)
{
    bool buttonPressed = false;

    if (event->type == SDL_CONTROLLERBUTTONDOWN) buttonPressed = true;

    ControllerButton buttonIndex = controllerMap[static_cast<SDL_GameControllerButton>(event->cbutton.button)];
    buttonsPressed[event->cbutton.which][static_cast<int>(buttonIndex)] = buttonPressed;
}

void Controller::connectController(const SDL_Event* event)
{
    Log::write("Input", LogLevel::info, "Controller connected");

    controller[event->cdevice.which] = SDL_GameControllerOpen(event->cdevice.which);
}

void Controller::disconnectController(const SDL_Event* event)
{
    Log::write("Input", LogLevel::info, "Controller disconnected");

    SDL_GameControllerClose(controller[event->cdevice.which]);
    controller[event->cdevice.which] = nullptr;
}

void Controller::cleanUp()
{

}