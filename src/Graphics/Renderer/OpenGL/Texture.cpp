#include "Texture.hpp"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/gl.h>
#endif

#include "ErrorHandling.hpp"

Texture::Texture()
{
    init();
}

Texture::Texture(Texture&& other) noexcept
{
    rendererID = other.rendererID;
    other.rendererID = 0;

    size = other.size;
}

Texture::Texture(const SDL_Surface* surface) : Texture()
{
    create(surface);
}

void Texture::init()
{
    glCall(glGenTextures(1, &rendererID));
    glCall(glBindTexture(GL_TEXTURE_2D, rendererID));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
}

void Texture::create(const SDL_Surface* surface)
{
    init();
    size.x = surface->w;
    size.y = surface->h;
    glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels));
}

void Texture::bind(const unsigned int slot) const
{
    glCall(glActiveTexture(GL_TEXTURE0 + slot));
    glCall(glBindTexture(GL_TEXTURE_2D, rendererID));
}

uint32_t Texture::getID() const
{
    return rendererID;
}

Vector2i Texture::getSize() const
{
    return size;
}

void Texture::unbind()
{
    glCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::free()
{
    glCall(glDeleteTextures(1, &rendererID));
}

Texture::~Texture()
{
    free();
}