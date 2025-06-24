#include "VertexArray.hpp"

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/gl.h>
#endif

#include "ErrorHandling.hpp"

VertexArray::VertexArray()
{
    glCall(glGenVertexArrays(1, &rendererID));
}

void VertexArray::bind() const
{
    glCall(glBindVertexArray(rendererID));
}

void VertexArray::unbind()
{
    glCall(glBindVertexArray(0));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    bind();
    vb.bind();
    const auto& elements = layout.getElements();
    unsigned long offset = 0;
    for(size_t i = 0; i < elements.size(); i++)
    {
        glCall(glEnableVertexAttribArray(i));
        glCall(glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, layout.getStride(), (const void*)offset));
        offset += elements[i].count * VertexBufferElement::GetSizeOfType(elements[i].type);
    }
}

VertexArray::~VertexArray()
{
    glCall(glDeleteVertexArrays(1, &rendererID));
}
