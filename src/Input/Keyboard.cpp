#include "Keyboard.hpp"

#include <bitset>
#include <unordered_map>

#include <SDL2/SDL.h>

static std::unordered_map<SDL_Scancode, Key> keyMap;
static std::bitset<64> keysPressed;
static std::bitset<64> keysPressedOld;

void Keyboard::init()
{
    keyMap.insert({SDL_SCANCODE_UNKNOWN, Key::null});
    keyMap.insert({SDL_SCANCODE_A, Key::a});
    keyMap.insert({SDL_SCANCODE_B, Key::b});
    keyMap.insert({SDL_SCANCODE_C, Key::c});
    keyMap.insert({SDL_SCANCODE_D, Key::d});
    keyMap.insert({SDL_SCANCODE_E, Key::e});
    keyMap.insert({SDL_SCANCODE_F, Key::f});
    keyMap.insert({SDL_SCANCODE_G, Key::g});
    keyMap.insert({SDL_SCANCODE_H, Key::h});
    keyMap.insert({SDL_SCANCODE_I, Key::i});
    keyMap.insert({SDL_SCANCODE_J, Key::j});
    keyMap.insert({SDL_SCANCODE_K, Key::k});
    keyMap.insert({SDL_SCANCODE_L, Key::l});
    keyMap.insert({SDL_SCANCODE_M, Key::m});
    keyMap.insert({SDL_SCANCODE_N, Key::n});
    keyMap.insert({SDL_SCANCODE_O, Key::o});
    keyMap.insert({SDL_SCANCODE_P, Key::p});
    keyMap.insert({SDL_SCANCODE_Q, Key::q});
    keyMap.insert({SDL_SCANCODE_R, Key::r});
    keyMap.insert({SDL_SCANCODE_S, Key::s});
    keyMap.insert({SDL_SCANCODE_T, Key::t});
    keyMap.insert({SDL_SCANCODE_U, Key::u});
    keyMap.insert({SDL_SCANCODE_V, Key::v});
    keyMap.insert({SDL_SCANCODE_W, Key::w});
    keyMap.insert({SDL_SCANCODE_X, Key::x});
    keyMap.insert({SDL_SCANCODE_Y, Key::y});
    keyMap.insert({SDL_SCANCODE_Z, Key::z});
    keyMap.insert({SDL_SCANCODE_0, Key::a0});
    keyMap.insert({SDL_SCANCODE_1, Key::a1});
    keyMap.insert({SDL_SCANCODE_2, Key::a2});
    keyMap.insert({SDL_SCANCODE_3, Key::a3});
    keyMap.insert({SDL_SCANCODE_4, Key::a4});
    keyMap.insert({SDL_SCANCODE_5, Key::a5});
    keyMap.insert({SDL_SCANCODE_6, Key::a6});
    keyMap.insert({SDL_SCANCODE_7, Key::a7});
    keyMap.insert({SDL_SCANCODE_8, Key::a8});
    keyMap.insert({SDL_SCANCODE_9, Key::a9});
    keyMap.insert({SDL_SCANCODE_LEFT, Key::left});
    keyMap.insert({SDL_SCANCODE_RIGHT, Key::right});
    keyMap.insert({SDL_SCANCODE_UP, Key::up});
    keyMap.insert({SDL_SCANCODE_DOWN, Key::down});
    keyMap.insert({SDL_SCANCODE_RETURN, Key::enter});
    keyMap.insert({SDL_SCANCODE_SPACE, Key::space});
    keyMap.insert({SDL_SCANCODE_ESCAPE, Key::escape});
    keyMap.insert({SDL_SCANCODE_TAB, Key::tab});
    keyMap.insert({SDL_SCANCODE_LALT, Key::leftAlt});
    keyMap.insert({SDL_SCANCODE_RALT, Key::rightAlt});
    keyMap.insert({SDL_SCANCODE_LCTRL, Key::leftCrtl});
    keyMap.insert({SDL_SCANCODE_RCTRL, Key::rightCrtl});
    keyMap.insert({SDL_SCANCODE_LSHIFT, Key::leftShift});
    keyMap.insert({SDL_SCANCODE_RSHIFT, Key::rightShift});
    keyMap.insert({SDL_SCANCODE_MENU, Key::menu});
}

void Keyboard::handleInput(SDL_Event* event)
{
    bool keyPressed = false;

    if (event->type == SDL_KEYDOWN) keyPressed = true;

    Key keyIndex = keyMap[event->key.keysym.scancode];
    keysPressed[static_cast<int>(keyIndex)] = keyPressed;
}

void Keyboard::update()
{
    keysPressedOld = keysPressed;
}

bool Keyboard::isKeyDown(Key key)
{
    return keysPressed[static_cast<int>(key)];
}

bool Keyboard::isKeyPressed(Key key)
{
    return keysPressed[static_cast<int>(key)] && !keysPressedOld[static_cast<int>(key)];
}