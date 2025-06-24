#pragma once

#include <vector>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    bool normalized;

    static unsigned int GetSizeOfType(unsigned int type);
};

class VertexBufferLayout
{
public:
    VertexBufferLayout();

    template<typename T>
    void push(unsigned int count);

    template<float>
    void push(unsigned int count);

    template<unsigned int>
    void push(unsigned int count);

    template<unsigned char>
    void push(unsigned int count);

    std::vector<VertexBufferElement> getElements() const;

    unsigned int getStride() const;

    ~VertexBufferLayout();
    
private:
    unsigned int stride;
    std::vector<VertexBufferElement> elements;
};
