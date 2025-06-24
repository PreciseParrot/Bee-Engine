#pragma once

class VertexBuffer
{
public:
    VertexBuffer();
    VertexBuffer(const void* data, unsigned int size);
    void init(const void* data, unsigned int size);
    void bind() const;
    static void unbind();
    ~VertexBuffer();

private:
    unsigned int rendererID;
};