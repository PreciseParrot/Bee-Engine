#include "Bee/Graphics/Window.hpp"
#include "Window-Internal.hpp"

#include <SDL2/SDL.h>

#include "Renderer-Internal.hpp"
#include "Bee/Log.hpp"

static SDL_Window* window = nullptr;

void Window::init(int windowWidth, int windowHeight)
{
    window = SDL_CreateWindow("Bee Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        Log::write("Renderer", LogLevel::error, "Error creating Window: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_SetWindowResizable(window, SDL_TRUE);
}

SDL_Window* Window::getWindow()
{
    return window;
}

Vector2i Window::getWindowSize()
{
    Vector2i size;
    SDL_GetWindowSize(window, &size.x, &size.y);
    
    return size;
}

void Window::setFullscreen(const bool fullscreen)
{
    if (fullscreen)
    {
        SDL_Rect displaySize;
        SDL_Event event;
        SDL_GetDisplayBounds(SDL_GetWindowDisplayIndex(window), &displaySize);
        event.window.event = SDL_WINDOWEVENT_RESIZED;
        event.window.data1 = displaySize.w;
        event.window.data2 = displaySize.h;
        Renderer::handleEvent(&event);

        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    else
    {
        SDL_SetWindowFullscreen(window, 0);
    }
}

void Window::setWindowIcon(const std::string& path)
{
    SDL_Surface* surface = Renderer::loadSurface(path);
    if (surface == nullptr)
    {
        Log::write("Renderer", LogLevel::error, "Can't load image: %s", SDL_GetError());
        return;
    }
    SDL_SetWindowIcon(window, surface);
    delete[] static_cast<unsigned char*>(surface->pixels);
    SDL_FreeSurface(surface);
}

void Window::setWindowTitle(const std::string& title)
{
    SDL_SetWindowTitle(window, title.c_str());
}

void Window::cleanUp()
{
    SDL_DestroyWindow(window);
}