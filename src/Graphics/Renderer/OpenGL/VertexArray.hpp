#pragma once

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray
{
public:
    VertexArray();
    void bind() const;
    static void unbind();
    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    ~VertexArray();

private:
    unsigned int rendererID{};
};