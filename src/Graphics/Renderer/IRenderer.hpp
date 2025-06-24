#pragma once

#include <SDL2/SDL.h>

#include "Bee/Entity.hpp"
#include "Bee/Graphics/HUDObject.hpp"
#include "Bee/Math/Vector2f.hpp"
#include "Graphics/Rect.hpp"

class IRenderer 
{
public:
    virtual void update() = 0;
    virtual int createTexture(const SDL_Surface* surface) = 0;
    virtual void freeTexture(int textureID) = 0;
    virtual int loadShader(const std::string& shader) = 0;
    virtual void queueTile(const Vector3f& position, int textureID, const Rect& rect) = 0;
    virtual void queueEntity(const Vector3f& position, const Vector2f& scale, int shaderID, int textureID, const Rect& rect, Entity* entity) = 0;
    virtual void queueHUD(const Vector3f& position, const Vector2f& scale, int shaderID, int textureID, const Rect& rect, HUDObject* hudObject) = 0;
    virtual void resize(const Vector2i& size) = 0;
    virtual Vector2f getCameraPosition() = 0;
    virtual Vector2f getViewportSize() = 0;
    virtual Vector2f getScreenSize() = 0;
    virtual Vector2f getTextureSize(int textureID) = 0;
    virtual void setCameraPosition(const Vector2f& position) = 0;
    virtual void setPostProcessingShader(const std::string& shader) = 0;
    virtual void setViewportSize(const Vector2f& size) = 0;
    virtual void setOnFrameCB(void(*func)()) = 0;
    virtual void setUniform1f(const std::string& name, float data) = 0;
    virtual void setUniform2f(const std::string& name, const Vector2f& data) = 0;
    virtual void setUniform3f(const std::string& name, const Vector3f& data) = 0;
    virtual void setUniform4f(const std::string& name, const Vector4f& data) = 0;
    virtual void setUniformMat4f(const std::string& name, const Matrix4f& matrix) = 0;
    virtual ~IRenderer() = default;
};