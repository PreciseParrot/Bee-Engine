#pragma once

#include <string>

#include "Graphics/Sprite.hpp"
#include "Math/Vector2f.hpp"
#include "Math/Vector2i.hpp"

class HUDObject
{
public:
    HUDObject();
    void updateInternal();
    void setAnimation(std::string animationName);
    void loadSpriteSheet(std::string spriteName);
    void setPosition(int x, int y);
    void setPosition(const Vector2i& position);
    void setScale(int width, int height);
    void setScale(const Vector2i& scale);
    virtual void update();
    virtual ~HUDObject();

private:
    float rotation = 0;
    std::string name;
    Sprite* sprite = nullptr;
    Vector2f rotationCenter = {0.5f, 0.5f};
    Vector2i position;
    Vector2i scale;
};