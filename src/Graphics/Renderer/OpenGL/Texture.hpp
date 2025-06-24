#pragma once

#include <SDL2/SDL.h>

#include "Bee/Math/Vector2i.hpp"

class Texture
{
public:
    Texture();
    Texture(Texture&& other) noexcept;
    Texture(const SDL_Surface* surface);
    void init();
    void create(const SDL_Surface* surface);
    void bind(unsigned int slot = 0) const;
    uint32_t getID() const;
    Vector2i getSize() const;
    static void unbind();
    void free();
    ~Texture();

private:
    unsigned int rendererID = 0;
    Vector2i size;
};
