#include "OpenGL.hpp"

#include <algorithm>
#include <array>

#include "Bee/Log.hpp"
#include "Bee/Math/Math.hpp"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/gl.h>
#endif

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include "Bee/Graphics/Window.hpp"
#include "Graphics/Window-Internal.hpp"
#include "ErrorHandling.hpp"
#include "BasicShader.hpp"

OpenGL::OpenGL()
{
#ifdef __EMSCRIPTEN__
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

    glContext = SDL_GL_CreateContext(Window::getWindow());
    SDL_GL_SetSwapInterval(0);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForOpenGL(Window::getWindow(), glContext);
    
#ifndef __EMSCRIPTEN__
    gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress));
#endif

    Log::write("Renderer", LogLevel::info, "OpenGL %s", glGetString(GL_VERSION));
    
    glCall(glEnable(GL_BLEND));
    glCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    glCall(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &textureSlots));
    
    const uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    quadIBO.init(indices, 6);

    textures.emplace_back();
    shaders.emplace_back(basicShaderVertSrc, basicShaderFragSrc);
    shaders.emplace_back(tileShaderVertSrc, tileShaderFragSrc);
    shaders.emplace_back(frameShaderVertSrc, frameShaderFragSrc);
    
    ImGui_ImplOpenGL3_Init("#version 300 es");
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void OpenGL::update()
{
    viewMatrix = Matrix4f(1.0f);
    viewMatrix.translate(-cameraPosition);

    worldProjectionMatrix = Math::ortho
    (
        -viewportSize.x / 2,
        viewportSize.x / 2,
        viewportSize.y / 2,
        -viewportSize.y / 2,
        -100.0f, 100.0f
    );

    screenProjectionMatrix = Math::ortho
    (
        0,
        screenSize.x,
        screenSize.y,
        0,
        -100.0f,
        100.0f
    );
    
    for (const auto& [layer, batch]: layersToDraw)
    {
        renderTiles(batch);
        renderEntities(batch);
        renderHUD(batch);
    }

    layersToDraw.clear();

    glCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    glCall(glBindTexture(GL_TEXTURE_2D, frameBufferTexture));

    const std::array<std::pair<Vector2f, Vector2f>, 4> rectVertices =
    {
        {
            {
                {1.0f, -1.0f},
                {1.0f, 0.0f}
            },
            {
                {-1.0f, -1.0f},
                {0.0f, 0.0f}
            },
            {
                {-1.0f, 1.0f},
                {0.0f, 1.0f}
            },
            {
                {1.0f, 1.0f},
                {1.0f, 1.0f}
            }
        }
    };

    const VertexBuffer vbo(&rectVertices, sizeof(rectVertices));

    VertexBufferLayout layout;
    layout.push<float>(2);
    layout.push<float>(2);

    VertexArray vao;
    vao.addBuffer(vbo, layout);

    const Vector2i windowSize = Window::getWindowSize();
    const Vector2f offset = (windowSize - screenSize) / 2.0f;

    glCall(glViewport(offset.x, offset.y, screenSize.x, screenSize.y));

    shaders.at(frameShaderID).bind();
    currentShader = &shaders.at(frameShaderID);

    if (onFrameCB)
        onFrameCB();

    draw(vao, quadIBO, *currentShader);

    glCall(glViewport(0, 0, screenSize.x, screenSize.y));

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(Window::getWindow());

#ifndef __EMSCRIPTEN__
    glCall(glClear(GL_COLOR_BUFFER_BIT));
#endif
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
    glCall(glClear(GL_COLOR_BUFFER_BIT));

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

int OpenGL::createTexture(const SDL_Surface* surface)
{
    int textureID = 0;

    if (freeTextures.empty())
    {
        textures.emplace_back(surface);
        textureID = textures.size() - 1;
    }
    else
    {
        textureID = freeTextures.back();
        freeTextures.pop_back();
        textures.at(textureID).create(surface);
    }

    return textureID;
}

void OpenGL::freeTexture(const int textureID)
{
    textures.at(textureID).free();
    freeTextures.push_back(textureID);
}

int OpenGL::loadShader(const std::string& shader)
{
    if (shader.empty()) return 0;

    if (shaderCache.contains(shader))
        return shaderCache.at(shader);

    const std::string shaderPath = "./assets/Shaders/" + shader + ".frag";
    FILE* fragmentShaderFile = fopen(shaderPath.c_str(), "r");

    fseek(fragmentShaderFile, 0, SEEK_END);
    const long fragmentShaderSize = ftell(fragmentShaderFile);
    fseek(fragmentShaderFile, 0, SEEK_SET);

    char* fragmentShaderSrc = static_cast<char*>(calloc(fragmentShaderSize + 1, sizeof(char)));
    fread(fragmentShaderSrc, 1, fragmentShaderSize, fragmentShaderFile);

    fclose(fragmentShaderFile);

    shaders.emplace_back(basicShaderVertSrc, fragmentShaderSrc);

    free(fragmentShaderSrc);
    int shaderID = shaders.size() - 1;
    shaderCache.insert({shader, shaderID});
    return shaderID;
}

void OpenGL::queueTile(const Vector3f& position, const int textureID, const Rect& rect)
{
    Batch& batch = layersToDraw[position.z];

    const auto it = std::ranges::find(batch.tileBatchTextureIDs, textureID);
    std::vector<std::array<Vertex, 4> >* vertices = nullptr;

    if (it == batch.tileBatchTextureIDs.end())
    {
        batch.tileBatchTextureIDs.push_back(textureID);
        batch.tileBatchVertices.emplace_back();
        vertices = &batch.tileBatchVertices.back();
    }
    else
    {
        size_t index = std::distance(batch.tileBatchTextureIDs.begin(), it);
        vertices = &batch.tileBatchVertices.at(index);
    }

    Vector2i textureSize = textures.at(textureID).getSize();

    float texelOffsetWidth = 0.1f / textureSize.x;
    float texelOffsetHeight = 0.1f / textureSize.y;

    vertices->emplace_back(std::array
    {
        Vertex
        {
            {position.x, position.y, position.z},
            {(rect.x + texelOffsetWidth) / textureSize.x, (rect.y + texelOffsetHeight) / textureSize.y}
        },
        Vertex
        {
            {position.x + 1, position.y, position.z},
            {(rect.x + rect.w) / textureSize.x, (rect.y + texelOffsetHeight) / textureSize.y}
        },
        Vertex
        {
            {position.x + 1, position.y + 1, position.z},
            {(rect.x + rect.w) / textureSize.x, (rect.y + rect.h) / textureSize.y}
        },
        Vertex
        {
            {position.x, position.y + 1, position.z},
            {(rect.x + texelOffsetWidth) / textureSize.x, (rect.y + rect.h) / textureSize.y}
        }
    });
}

void OpenGL::queueEntity(const Vector3f& position, const Vector2f& scale, int shaderID, const int textureID, const Rect& rect, Entity* entity)
{
    Batch& batch = layersToDraw[position.z];

    Matrix4f modelMatrix(1.0f);
    modelMatrix.scale(scale);
    modelMatrix.translate(position);

    const Vector2i textureSize = textures.at(textureID).getSize();

    const float texelOffsetWidth = 0.1f / textureSize.x;
    const float texelOffsetHeight = 0.1f / textureSize.y;

    batch.entitySprites.emplace_back(shaderID, textureID, entity, modelMatrix, std::array
    {
        Vertex
        {
            {-0.5, -0.5, 0},
            {
                (rect.x + texelOffsetWidth) / textureSize.x,
                (rect.y + texelOffsetHeight) / textureSize.y
            },
            {1.0f, 1.0f, 1.0f, 1.0f}
        },
        Vertex
        {
            {0.5, -0.5, 0},
            {
                (rect.x + rect.w) / textureSize.x,
                (rect.y + texelOffsetHeight) / textureSize.y
            },
            {1.0f, 1.0f, 1.0f, 1.0f}
        },
        Vertex
        {
            {0.5, 0.5, 0},
            {
                (rect.x + rect.w) / textureSize.x,
                (rect.y + rect.h) / textureSize.y
            },
            {1.0f, 1.0f, 1.0f, 1.0f}
        },
        Vertex
        {
            {-0.5, 0.5, 0},
            {
                (rect.x + texelOffsetWidth) / textureSize.x,
                (rect.y + rect.h) / textureSize.y
            },
            {1.0f, 1.0f, 1.0f, 1.0f}
        }
    });
}

void OpenGL::queueHUD(const Vector3f& position, const Vector2f& scale, int shaderID, int textureID, const Rect& rect, HUDObject* hudObject)
{
    Batch& batch = layersToDraw[position.z];

    Matrix4f modelMatrix(1.0f);
    modelMatrix.scale(scale);
    modelMatrix.translate(position);

    Matrix4f mvp = modelMatrix * screenProjectionMatrix;

    const Vector2i textureSize = textures.at(textureID).getSize();

    const float texelOffsetWidth = 0.1f / textureSize.x;
    const float texelOffsetHeight = 0.1f / textureSize.y;

    batch.hudSprites.emplace_back(shaderID, textureID, hudObject, mvp, std::array
    {
        Vertex
        {
            {0, 0, 0},
            {
                (rect.x + texelOffsetWidth) / textureSize.x,
                (rect.y + texelOffsetHeight) / textureSize.y
            },
            {1.0f, 1.0f, 1.0f, 1.0f}
        },
        Vertex
        {
            {1, 0, 0},
            {
                (rect.x + rect.w) / textureSize.x,
                (rect.y + texelOffsetHeight) / textureSize.y
            },
            {1.0f, 1.0f, 1.0f, 1.0f}
        },
        Vertex
        {
            {1, 1, 0},
            {
                (rect.x + rect.w) / textureSize.x,
                (rect.y + rect.h) / textureSize.y
            },
            {1.0f, 1.0f, 1.0f, 1.0f}
        },
        Vertex
        {
            {0, 1, 0},
            {
                (rect.x + texelOffsetWidth) / textureSize.x,
                (rect.y + rect.h) / textureSize.y
            },
            {1.0f, 1.0f, 1.0f, 1.0f}
        }
    });
}

void OpenGL::resize(const Vector2i& size)
{
    const float widthFactor = size.x / viewportSize.x;
    const float heightFactor = size.y / viewportSize.y;

    if (widthFactor > heightFactor)
    {
        screenSize.x = size.x * heightFactor / widthFactor;
        screenSize.y = size.y;
    }
    else
    {
        screenSize.x = size.x;
        screenSize.y = size.y * widthFactor / heightFactor;
    }

    glCall(glDeleteFramebuffers(1, &fbo));
    glCall(glDeleteRenderbuffers(1, &rbo));
    glCall(glDeleteTextures(1, &frameBufferTexture));

    glCall(glGenFramebuffers(1, &fbo));
    glCall(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

    glCall(glGenTextures(1, &frameBufferTexture));
    glCall(glBindTexture(GL_TEXTURE_2D, frameBufferTexture));
    glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenSize.x, screenSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    glCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0));

    glCall(glGenRenderbuffers(1, &rbo));
    glCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));
    glCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenSize.x, screenSize.y));
    glCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));
}

Vector2f OpenGL::getCameraPosition()
{
    return cameraPosition;
}

Vector2f OpenGL::getScreenSize()
{
    return screenSize;
}

Vector2f OpenGL::getViewportSize()
{
    return viewportSize;
}

Vector2f OpenGL::getTextureSize(int textureID)
{
    return textures.at(textureID).getSize();
}

void OpenGL::setCameraPosition(const Vector2f& position)
{
    cameraPosition = position;
}

void OpenGL::setPostProcessingShader(const std::string& shader)
{
    if (shader.empty())
    {
        frameShaderID = 2;
        return;
    }

    if (shaderCache.contains(shader))
        frameShaderID = shaderCache.at(shader);

    const std::string shaderPath = "./assets/Shaders/" + shader + ".frag";
    FILE* fragmentShaderFile = fopen(shaderPath.c_str(), "r");

    fseek(fragmentShaderFile, 0, SEEK_END);
    const long fragmentShaderSize = ftell(fragmentShaderFile);
    fseek(fragmentShaderFile, 0, SEEK_SET);

    char* fragmentShaderSrc = static_cast<char*>(calloc(fragmentShaderSize + 1, sizeof(char)));
    fread(fragmentShaderSrc, 1, fragmentShaderSize, fragmentShaderFile);

    fclose(fragmentShaderFile);
    
    shaders.emplace_back(frameShaderVertSrc, fragmentShaderSrc);
    
    free(fragmentShaderSrc);
    int shaderID = shaders.size() - 1;
    frameShaderID = shaderID;
    shaderCache.insert({shader, shaderID});
}

void OpenGL::setViewportSize(const Vector2f& size)
{
    viewportSize = size;
}

void OpenGL::setOnFrameCB(void(*func)())
{
    onFrameCB = func;
}

void OpenGL::setUniform1f(const std::string& name, float data)
{
    currentShader->setUniform1f(name, data);
}

void OpenGL::setUniform2f(const std::string& name, const Vector2f& data)
{
    currentShader->setUniform2f(name, data);
}

void OpenGL::setUniform3f(const std::string& name, const Vector3f& data)
{
    currentShader->setUniform3f(name, data);
}

void OpenGL::setUniform4f(const std::string& name, const Vector4f& data)
{
    currentShader->setUniform4f(name, data);
}

void OpenGL::setUniformMat4f(const std::string& name, const Matrix4f& matrix)
{
    currentShader->setUniformMat4f(name, matrix);
}

void OpenGL::renderEntities(const Batch& batch)
{
    for (const Sprite& sprite: batch.entitySprites)
    {
        Shader& shader = shaders.at(sprite.shaderID);
        shader.bind();
        currentShader = &shader;
        shader.setUniformMat4f("u_MVP", sprite.modelMatrix * viewMatrix * worldProjectionMatrix);

        textures.at(sprite.texID).bind(0);

        VertexBuffer vbo(&sprite.vertices, sizeof(sprite.vertices));

        VertexBufferLayout layout;
        layout.push<float>(3);
        layout.push<float>(2);
        layout.push<float>(4);

        VertexArray vao;
        vao.addBuffer(vbo, layout);

        Entity* entity = static_cast<Entity*>(sprite.object);
        entity->onDraw();

        draw(vao, quadIBO, shaders.at(sprite.shaderID));
    }
}

void OpenGL::renderHUD(const Batch& batch)
{
    for (const Sprite& sprite: batch.hudSprites)
    {
        Shader& shader = shaders.at(sprite.shaderID);
        shader.bind();
        currentShader = &shader;
        shader.setUniformMat4f("u_MVP", sprite.modelMatrix);

        textures.at(sprite.texID).bind(0);

        VertexBuffer vbo(&sprite.vertices, sizeof(sprite.vertices));

        VertexBufferLayout layout;
        layout.push<float>(3);
        layout.push<float>(2);
        layout.push<float>(4);

        VertexArray vao;
        vao.addBuffer(vbo, layout);

        HUDObject* hudObject = static_cast<HUDObject*>(sprite.object);
        hudObject->onDraw();

        draw(vao, quadIBO, shaders.at(sprite.shaderID));
    }
}

void OpenGL::renderTiles(const Batch& batch)
{
    for (size_t i = 0; i < batch.tileBatchVertices.size(); i++)
    {
        std::vector<int> indices;

        for (size_t j = 0; j < batch.tileBatchVertices.at(i).size(); j++)
        {
            indices.push_back(0 + j * 4);
            indices.push_back(1 + j * 4);
            indices.push_back(2 + j * 4);
            indices.push_back(2 + j * 4);
            indices.push_back(3 + j * 4);
            indices.push_back(0 + j * 4);
        }

        IndexBuffer ibo(indices.data(), indices.size());

        Matrix4f mvp = viewMatrix * worldProjectionMatrix;

        Shader& shader = shaders.at(1);
        shader.bind();
        currentShader = &shader;
        shader.setUniformMat4f("u_MVP", mvp);

        textures.at(batch.tileBatchTextureIDs.at(i)).bind();

        VertexBuffer vbo(batch.tileBatchVertices.at(i).data(), batch.tileBatchVertices.at(i).size() * sizeof(std::array<Vertex, 4>));

        VertexBufferLayout layout;
        layout.push<float>(3);
        layout.push<float>(2);
        layout.push<float>(4);

        VertexArray vao;
        vao.addBuffer(vbo, layout);

        draw(vao, ibo, shader);
    }
}

void OpenGL::draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader)
{
    shader.bind();
    vertexArray.bind();
    indexBuffer.bind();
    glCall(glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr));
}

OpenGL::~OpenGL()
{
    SDL_GL_DeleteContext(glContext);
}
