#include "Bee/Graphics/Renderer.hpp"
#include "Renderer-Internal.hpp"

#include <cstdio>
#include <filesystem>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <png.h>

#include "Window-Internal.hpp"
#include "Graphics/Renderer/IRenderer.hpp"
#include "Graphics/Renderer/OpenGL/OpenGL.hpp"
#include "Bee/Log.hpp"
#include "Bee/Math/Vector2f.hpp"
#include "Bee/Math/Vector2i.hpp"

static std::map<std::pair<std::string, int>, TTF_Font*> fontMap;
static std::unordered_map<std::string, int> textureCache;
static std::unordered_set<int> uniqueTextures;
static IRenderer* renderer = nullptr;

void Renderer::init(const int windowWidth, const int windowHeight)
{
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        Log::write("Renderer", LogLevel::error, "Error initializing video system: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() == -1)
    {
        Log::write("Renderer", LogLevel::error, "Error initializing SDL2_ttf: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    
    Window::init(windowWidth, windowHeight);
    
    renderer = new OpenGL;

    SDL_Event event;
    event.window.event = SDL_WINDOWEVENT_RESIZED;
    event.window.data1 = windowWidth;
    event.window.data2 = windowHeight;
    handleEvent(&event);

    Log::write("Renderer", LogLevel::info, "Initialized renderer");
}

void Renderer::update()
{
    renderer->update();
}

void Renderer::handleEvent(const SDL_Event* event)
{
    if (event->window.event == SDL_WINDOWEVENT_RESIZED)
    {
        renderer->resize({event->window.data1, event->window.data2});
    }
}

void Renderer::queueTile(const Vector3f& position, int textureID, const Rect& rect)
{
    if (!textureID) return;
    renderer->queueTile(position, textureID, rect);
}

void Renderer::queueHUD(const Vector3f& position, const Vector2f& scale, int shaderID, int textureID, const Rect& rect, HUDObject* hudObject)
{
    if (!textureID) return;
    renderer->queueHUD(position, scale, shaderID, textureID, rect, hudObject);
}

void Renderer::queueEntity(const Vector3f& position, const Vector2f& scale, int shaderID, int textureID, const Rect& rect, Entity* entity)
{
    if (!textureID) return;
    renderer->queueEntity(position, scale, shaderID, textureID, rect, entity);
}

int Renderer::loadShader(const std::string& shader)
{
    return renderer->loadShader(shader);
}

int Renderer::loadTexture(const std::string& textureName, const std::string& path)
{
    if (textureCache.contains(textureName))
        return textureCache.at(textureName);

    SDL_Surface* surface = loadSurface(path);

    if (!surface)
    {
        return 0;
    }

    const int textureID = renderer->createTexture(surface);
    delete[] static_cast<unsigned char*>(surface->pixels);
    SDL_FreeSurface(surface);
    return textureID;
}

int Renderer::createUniqueTexture(const SDL_Surface* surface)
{
    const int textureID = renderer->createTexture(surface);
    uniqueTextures.insert(textureID);
    return textureID;
}

SDL_Surface* Renderer::loadSurface(const std::string& path)
{
    FILE* file = nullptr;
    png_structp png = nullptr;
    png_infop info = nullptr;
    SDL_Surface* surface = nullptr;
    png_bytep* row_pointers = nullptr;
    unsigned char* data = nullptr;
    unsigned int width = 0;
    unsigned int height = 0;

    file  = fopen(path.c_str(), "rb");
    if (!file)
    {
        Log::write("Renderer", LogLevel::warning, "Can't load texture: %s / file not found", path.c_str());
        goto Error;
    }

    png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png)
    {
        Log::write("Renderer", LogLevel::warning, "Can't load texture: %s / error parsing data", path.c_str());
        goto Error;
    }

    info = png_create_info_struct(png);
    if (!info)
    {
        Log::write("Renderer", LogLevel::warning, "Can't load texture: %s / error parsing data", path.c_str());
        goto Error;
    }

    if (setjmp(png_jmpbuf(png)))
    {
        goto Error;
    }

    png_init_io(png, file);
    png_read_info(png, info);

    width = png_get_image_width(png, info);
    height = png_get_image_height(png, info);
    data = new unsigned char[png_get_rowbytes(png, info) * height];
    row_pointers = new png_bytep[height];

    for (unsigned int y = 0; y < height; y++)
    {
        row_pointers[y] = data + y * png_get_rowbytes(png, info);
    }

    png_read_image(png, row_pointers);

    surface = SDL_CreateRGBSurfaceFrom(data, width, height, 32, width * 4, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

Error:
    fclose(file);
    png_destroy_read_struct(&png, nullptr, nullptr);
    delete[] row_pointers;
    return surface;
}

TTF_Font* Renderer::loadFont(const std::string& fontName, int size)
{
    if (fontMap.contains({fontName, size}))
    {
        return fontMap[{fontName, size}];
    }

    std::string path = "./assets/Fonts/" + fontName + ".ttf";

    if (!std::filesystem::exists(path))
    {
        path = "./assets/Fonts/" + fontName + ".otf";
    }

    TTF_Font* font = TTF_OpenFont(path.c_str(), size);

    if (font == nullptr)
    {
        Log::write("Renderer", LogLevel::error, "Can't load font: %s / %s", fontName.c_str(), SDL_GetError());
    }
    else
    {
        Log::write("Renderer", LogLevel::info, "Loaded %s font with size %i", fontName.c_str(), size);
        fontMap.insert({{fontName, size}, font});
    }
    return font;
}

void Renderer::deleteUniqueTexture(const int textureID)
{
    if (uniqueTextures.contains(textureID))
    {
        uniqueTextures.erase(textureID);
        renderer->freeTexture(textureID);
    }
}

Vector2f Renderer::getCameraPosition()
{
    return renderer->getCameraPosition();
}

Vector2i Renderer::getScreenSize()
{
    return renderer->getScreenSize();
}

Vector2f Renderer::getTextureSize(const int textureID)
{
    return renderer->getTextureSize(textureID);
}

Vector2f Renderer::getViewPortSize()
{
    return renderer->getViewportSize();
}

void Renderer::setCameraPosition(const Vector2f& position)
{
    renderer->setCameraPosition(position);
}

void Renderer::setShader(const std::string& shader)
{
    renderer->setPostProcessingShader(shader);
}

void Renderer::setViewportSize(const Vector2f& size)
{
    renderer->setViewportSize(size);
}

void Renderer::setOnFrameCB(void(*func)())
{
    renderer->setOnFrameCB(func);
}

void Renderer::setUniform1f(const std::string& name, const float data)
{
    renderer->setUniform1f(name, data);
}

void Renderer::setUniform2f(const std::string& name, const Vector2f& data)
{
    renderer->setUniform2f(name, data);
}

void Renderer::setUniform3f(const std::string& name, const Vector3f& data)
{
    renderer->setUniform3f(name, data);
}

void Renderer::setUniform4f(const std::string& name, const Vector4f& data)
{
    renderer->setUniform4f(name, data);
}

void Renderer::setUniformMat4f(const std::string& name, const Matrix4f& matrix)
{
    renderer->setUniformMat4f(name, matrix);
}

void Renderer::cleanUp()
{
    delete renderer;
    TTF_Quit();
}
