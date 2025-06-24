#include "VertexBufferLayout.hpp"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/gl.h>
#endif

#include "ErrorHandling.hpp"

unsigned int VertexBufferElement::GetSizeOfType(unsigned int type)
{
    switch (type)
    {
        case GL_FLOAT:
        case GL_UNSIGNED_INT:   return 4;
        case GL_UNSIGNED_BYTE:  return 1;
        default: break;
    }
    assert(false);
    return 0;
}

VertexBufferLayout::VertexBufferLayout()
    : stride(0)
{

}

template<typename T>
void VertexBufferLayout::push(unsigned int count)
{
    static_assert(false);
}

template<>
void VertexBufferLayout::push<float>(unsigned int count)
{
    elements.push_back({GL_FLOAT, count, GL_FALSE});
    stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}

template<>
void VertexBufferLayout::push<unsigned int>(unsigned int count)
{
    elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
}

template<>
void VertexBufferLayout::push<unsigned char>(unsigned int count)
{
    elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}

std::vector<VertexBufferElement> VertexBufferLayout::getElements() const
{
    return elements;
}

unsigned int VertexBufferLayout::getStride() const
{
    return stride;
}

VertexBufferLayout::~VertexBufferLayout() = default;