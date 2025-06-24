#include "VertexBuffer.hpp"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/gl.h>
#endif

#include "ErrorHandling.hpp"

VertexBuffer::VertexBuffer()
{
    rendererID = 0;
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size) : VertexBuffer()
{
    init(data, size);
}

void VertexBuffer::init(const void *data, unsigned int size)
{
    glCall(glGenBuffers(1, &rendererID));
    glCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
    glCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::bind() const
{
    glCall(glBindBuffer(GL_ARRAY_BUFFER, rendererID));
}

void VertexBuffer::unbind()
{
    glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &rendererID);
}