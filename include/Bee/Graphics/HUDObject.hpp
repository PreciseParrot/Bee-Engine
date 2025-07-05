#pragma once

#include "Bee/BaseObject.hpp"

class HUDObject : public BaseObject
{
public:
    bool isCursorOnMe() const;
    void setScale(float scale);
    void setSprite(const std::string& spriteName);
    void setText(const std::string& text, const std::string& font, int fontSize, const Color& color);
    void update() override;
};
