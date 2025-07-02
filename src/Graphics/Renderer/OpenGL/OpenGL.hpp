#pragma once

#include <array>
#include <map>
#include <vector>

#include <SDL2/SDL.h>

#include "IndexBuffer.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include "Graphics/Renderer/IRenderer.hpp"

#include "Bee/Graphics/Color.hpp"
#include "Bee/Math/Vector3f.hpp"

class OpenGL final : public IRenderer
{
public:
    OpenGL();
    void update() override;
    int createTexture(const SDL_Surface* surface) override;
    void freeTexture(int textureID) override;
    int loadShader(const std::string& shader) override;
    void queueTile(const Vector3f& position, int textureID, const Rect& rect) override;
    void queueEntity(const Vector3f& position, const Vector2f& scale, int shaderID, int textureID, const Rect& rect, Entity* entity) override;
    void queueHUD(const Vector3f& position, const Vector2f& scale, int shaderID, int textureID, const Rect& rect, HUDObject* hudObject) override;
    void resize(const Vector2i& size) override;
    Vector2f getCameraPosition() override;
    Vector2f getScreenSize() override;
    Vector2f getViewportSize() override;
    Vector2f getTextureSize(int textureID) override;
    void setCameraPosition(const Vector2f& position) override;
    void setPostProcessingShader(const std::string& shader) override;
    void setViewportSize(const Vector2f& size) override;
    void setOnFrameCB(void(*func)()) override;
    void setUniform1f(const std::string& name, float data) override;
    void setUniform2f(const std::string& name, const Vector2f& data) override;
    void setUniform3f(const std::string& name, const Vector3f& data) override;
    void setUniform4f(const std::string& name, const Vector4f& data) override;
    void setUniformMat4f(const std::string& name, const Matrix4f& matrix) override;
    ~OpenGL() override;

private:
    int textureSlots = 0;
    uint32_t fbo = 0;
    uint32_t rbo = 0;
    uint32_t frameBufferTexture = 0;
    Vector2f cameraPosition;
    Vector2i screenSize;
    Vector2f viewportSize {16, 9};
    std::vector<Texture> textures;
    std::vector<int> freeTextures;
    std::vector<Shader> shaders;
    std::unordered_map<std::string, int> shaderCache;
    int frameShaderID = 2;
    Shader* currentShader = nullptr;
    void (*onFrameCB)() = nullptr;
    SDL_GLContext glContext;

    struct Vertex
    {
        Vector3f position;
        Vector2f texCoord;
        Color color;
    };

    struct Sprite
    {
        int shaderID;
        int texID;
        void* object;
        Matrix4f modelMatrix;
        std::array<Vertex, 4> vertices;
    };

    struct Batch
    {
        std::vector<std::vector<std::array<Vertex, 4>>> tileBatchVertices;
        std::vector<int> tileBatchTextureIDs;

        std::vector<Sprite> entitySprites;

        std::vector<Sprite> hudSprites;
    };

    std::map<float, Batch> layersToDraw;

    Matrix4f viewMatrix;
    Matrix4f worldProjectionMatrix;
    Matrix4f screenProjectionMatrix;

    IndexBuffer quadIBO;

    void renderEntities(const Batch&);
    void renderHUD(const Batch&);
    void renderTiles(const Batch&);
    void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader);
};