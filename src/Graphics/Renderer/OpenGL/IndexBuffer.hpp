#pragma once

class IndexBuffer
{
public:
    IndexBuffer();
    IndexBuffer(const void* data, unsigned int count);
    void init(const void* data, unsigned int count);
    void bind() const;
    void unbind() const;
    unsigned int getCount() const { return count; } 
    ~IndexBuffer();

private:
    unsigned int rendererID;
    unsigned int count;
};
