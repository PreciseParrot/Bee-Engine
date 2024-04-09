#include "Controller.hpp"

#include <bitset>
#include <unordered_map>

#include <SDL2/SDL.h>

static std::unordered_map<SDL_GameControllerButton, ControllerButton> controllerMap;
static std::bitset<32> buttonsPressed;
static std::bitset<32> buttonsPressedOld;
static SDL_GameController* controller = nullptr;
static Vector2f controllerDeadzone(0.1f, 0.1f);

void Controller::init()
{
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
    controllerMap.insert({SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, ControllerButton::rigtShoulder});

    for (int i = 0; i < SDL_NumJoysticks(); i++)
    {
        if (SDL_IsGameController(i))
        {
            controller = SDL_GameControllerOpen(i);
        }
    }
}

bool Controller::isButtonDown(ControllerButton button)
{
    return buttonsPressed[static_cast<int>(button)];
}

bool Controller::isButtonPressed(ControllerButton button)
{
    return buttonsPressed[static_cast<int>(button)] && !buttonsPressedOld[static_cast<int>(button)];
}

Vector2f Controller::getLeftStick()
{
    Vector2f stick;

    stick.x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) / 32768.0f;
    stick.y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) / 32768.0f;

    if (stick.x < controllerDeadzone.x && stick.x > -controllerDeadzone.x) stick.x = 0;
    if (stick.y < controllerDeadzone.y && stick.y > -controllerDeadzone.y) stick.y = 0;

    return stick;
}

Vector2f Controller::getRightStick()
{
    Vector2f stick;

    stick.x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) / 32768.0f;
    stick.y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) / 32768.0f;

    if (stick.x < controllerDeadzone.x && stick.x > -controllerDeadzone.x) stick.x = 0;
    if (stick.y < controllerDeadzone.y && stick.y > -controllerDeadzone.y) stick.y = 0;
    
    return stick;
}

void Controller::update()
{
    buttonsPressedOld = buttonsPressed;

    if (controller)
    {
        if (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > 100)
        {
            buttonsPressed[static_cast<int>(ControllerButton::leftTrigger)] = true;
        }
        else
        {
            buttonsPressed[static_cast<int>(ControllerButton::leftTrigger)] = false;
        }

        if (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > 100)
        {
            buttonsPressed[static_cast<int>(ControllerButton::rightTrigger)] = true;
        }
        else
        {
            buttonsPressed[static_cast<int>(ControllerButton::rightTrigger)] = false;
        }
    }
}

void Controller::handleInput(SDL_Event* event)
{
    bool buttonPressed = false;

    if (event->type == SDL_CONTROLLERBUTTONDOWN) buttonPressed = true;

    ControllerButton buttonIndex = controllerMap[(SDL_GameControllerButton)event->jbutton.button];
    buttonsPressed[static_cast<int>(buttonIndex)] = buttonPressed;
}

void Controller::connectController(SDL_Event* event)
{
    if (!controller)
    {
        SDL_GameControllerOpen(event->cdevice.which);
    }
}

void Controller::disconnectController(SDL_Event* event)
{
    if (controller && event->cdevice.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller)))
    {
        SDL_GameControllerClose(controller);
        controller = nullptr;
    }
}

void Controller::cleanUp()
{
    SDL_GameControllerClose(controller);
}