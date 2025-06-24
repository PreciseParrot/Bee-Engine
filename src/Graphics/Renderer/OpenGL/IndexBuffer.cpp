#include "IndexBuffer.hpp"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/gl.h>
#endif

#include "ErrorHandling.hpp"

IndexBuffer::IndexBuffer() = default;

IndexBuffer::IndexBuffer(const void* data, const unsigned int count)
{
    init(data, count);
}

void IndexBuffer::init(const void *data, unsigned int count)
{
    assert(sizeof(GLuint) == sizeof(unsigned int));

    this->count = count;

    glCall(glGenBuffers(1, &rendererID));
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
    glCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

void IndexBuffer::bind() const
{
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID));
}

void IndexBuffer::unbind() const
{
    glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &rendererID);
}
