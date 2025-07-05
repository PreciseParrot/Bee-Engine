#pragma once

#include "Bee/BaseObject.hpp"
#include "Bee/Collision/Intersection.hpp"

class Entity : public BaseObject
{
public:
    bool isCursorOnMe() const;
    std::vector<Intersection> getIntersections() const;
    void setScale(float scale);
    void setSprite(const std::string& spriteName);
    void setText(const std::string& text, const std::string& font, int fontSize, const Color& color);
    void update() override;
};
