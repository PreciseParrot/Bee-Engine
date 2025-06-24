#include "Bee/Graphics/Color.hpp"

Color::Color()
{
    r = 0;
    g = 0;
    b = 0;
    a = 0;
}

Color::Color(float r, float g, float b, float a)
{
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}
