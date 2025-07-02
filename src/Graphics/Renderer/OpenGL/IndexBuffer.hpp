#pragma once

#include <cstdint>

class IndexBuffer
{
public:
    IndexBuffer();
    IndexBuffer(const void* data, uint32_t count);
    void init(const void* data, uint32_t count);
    void bind() const;
    static void unbind() ;
    uint32_t getCount() const;
    ~IndexBuffer();

private:
    uint32_t rendererID = 0;
    uint32_t count = 0;
};
