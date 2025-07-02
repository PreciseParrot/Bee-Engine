#include "IndexBuffer.hpp"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/gl.h>
#endif

#include "ErrorHandling.hpp"
#include "Bee/Log.hpp"

IndexBuffer::IndexBuffer() = default;

IndexBuffer::IndexBuffer(const void* data, const uint32_t count)
{
    init(data, count);
}

void IndexBuffer::init(const void *data, uint32_t count)
{
    assert(sizeof(GLuint) == sizeof(uint32_t));

    this->count = count;

    glCall(glGenBuffers(1, &rendererID));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW));
}

void IndexBuffer::bind() const
{
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
}

void IndexBuffer::unbind()
{
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

uint32_t IndexBuffer::getCount() const
{
    return count;
}

IndexBuffer::~IndexBuffer()
{
    glCall(glDeleteBuffers(1, &rendererID));
}
