#include "Sprite.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include <nlohmann/json.hpp>
#include <SDL2/SDL.h>

#include "Bee.hpp"
#include "Graphics/Renderer.hpp"
#include "Math/Vector2f.hpp"
#include "Math/Vector2i.hpp"

Sprite::Sprite() {}

void Sprite::loadSpriteSheet(std::string spriteName)
{
    std::string jsonFilePath = "./assets/Sprites/" + spriteName + ".json";
    std::string pngFilePath = "./assets/Sprites/" + spriteName + ".png";

    texture = Renderer::loadTexture(spriteName, pngFilePath);

    currentAnimation.start = 0;    
    currentAnimation.end = 0;
    currentAnimation.direction = ANIMATION_NONE;
    frameTags.insert({"no_animation", currentAnimation});

    std::ifstream jsonFile(jsonFilePath);
    
    if (jsonFile.fail())
        return;

    nlohmann::json spriteData = nlohmann::json::parse(jsonFile);

    for (const nlohmann::json& spriteFrameJson : spriteData["frames"])
    {
        AnimationSpriteFrame spriteFrame;
        spriteFrame.x = spriteFrameJson["frame"]["x"].get<int>();
        spriteFrame.y = spriteFrameJson["frame"]["y"].get<int>();
        spriteFrame.w = spriteFrameJson["frame"]["w"].get<int>();
        spriteFrame.h = spriteFrameJson["frame"]["h"].get<int>();
        spriteFrame.duration = spriteFrameJson["duration"].get<int>();

        sprites.push_back(spriteFrame);
    }

    for (const nlohmann::json& frameTagJson : spriteData["meta"]["frameTags"])
    {
        FrameTag frameTag;
        frameTag.start = frameTagJson["from"].get<int>();
        frameTag.end = frameTagJson["to"].get<int>();

        std::string direction = frameTagJson["direction"].get<std::string>();

        if (direction == "forward")
        {
            frameTag.direction = ANIMATION_FORWARD;
        }
        else if (direction == "reverse")
        {
            frameTag.direction = ANIMATION_REVERSE;
        }
        else if (direction == "pingpong")
        {
            frameTag.direction = ANIMATION_PINGPONG;
        }

        frameTags.insert({frameTagJson["name"].get<std::string>(), frameTag});
    }
}

void Sprite::setAnimation(std::string animationName)
{
    if (currentAnimationName == animationName) return;

    try
    {
        currentAnimation = frameTags.at(animationName);
    }
    catch(const std::exception& e)
    {
        std::cerr << "Animation: " << animationName << " not found\n";
    }

    if (currentAnimation.direction == ANIMATION_REVERSE)
    {
        currentAnimationDirection = ANIMATION_REVERSE;
        currentSprite = currentAnimation.end;
    }
    else
    {
        currentAnimationDirection = ANIMATION_FORWARD;
        currentSprite = currentAnimation.start;
    }
    currentAnimationName = animationName;
}

void Sprite::updateInternal()
{
    if (sprites.size() == 0 || currentAnimation.direction == ANIMATION_NONE)
        return;
  
    uint32_t currentTime = Bee::getTime();
    if (frameStartTime + sprites[currentSprite].duration <= currentTime)
    {
        frameStartTime = currentTime;

        if (currentAnimationDirection == ANIMATION_FORWARD)
        {
            currentSprite++;
        }
        else
        {
            currentSprite--;
        }

        if (currentAnimation.direction == ANIMATION_FORWARD)
        {
            currentAnimationDirection == ANIMATION_FORWARD;
            if (currentSprite > currentAnimation.end)
            {
                currentSprite = currentAnimation.start;
            }
        }
        else if (currentAnimation.direction == ANIMATION_REVERSE)
        {
            currentAnimationDirection == ANIMATION_REVERSE;
            if (currentSprite < currentAnimation.start)
            {
                currentSprite = currentAnimation.end;
            }
        }
        else if (currentAnimation.direction == ANIMATION_PINGPONG)
        {
            if (currentSprite >= currentAnimation.end && currentAnimationDirection == ANIMATION_FORWARD)
            {
                currentAnimationDirection = ANIMATION_REVERSE;
            }
            else if (currentSprite <= currentAnimation.start && currentAnimationDirection == ANIMATION_REVERSE)
            {
                currentAnimationDirection = ANIMATION_FORWARD;
            }
        }
    }
}

void Sprite::updateInternalHUD(const Vector2i& position, const Vector2i& scale, const Vector2f& rotationCenter, float rotation)
{
    updateInternal();
    Renderer::drawHUD(position, scale, (SDL_Rect*)&sprites[currentSprite], texture, rotationCenter, rotation);
}

void Sprite::updateInternalEntity(const Vector2f& position, const Vector2f& scale, const Vector2f& rotationCenter, float rotation)
{
    updateInternal();
    Renderer::drawSprite(position, scale, (SDL_Rect*)&sprites[currentSprite], texture, rotationCenter, rotation);
}

Sprite::~Sprite()
{

}