#include "Sprite.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <stdint.h>

#include <nlohmann/json.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Bee.hpp"
#include "Log.hpp"
#include "Renderer.hpp"
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
    
    if (jsonFile.fail()) return;

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

    if (frameTags.find(animationName) != frameTags.end())
    {
        currentAnimation = frameTags.at(animationName);
    }
    else
    {
        Log::write("Sprite", LogLevel::Warning, "Animation: %s not found", animationName);
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

void Sprite::setFont(std::string fontName, int size)
{
    font = Renderer::loadFont(fontName, size);
}

void Sprite::setText(std::string text, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    if (this->text == text) return;

    this->text = text;
    SDL_DestroyTexture(texture);

    SDL_Color color = {red, green, blue, alpha};
    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color, 0);
    texture = Renderer::createTexture(surface);
    SDL_FreeSurface(surface);
}

Vector2i Sprite::getTextureSize()
{
    Vector2i textureSize;

    if (sprites.size() == 0)
    {
        SDL_QueryTexture(texture, NULL, NULL, &textureSize.x, &textureSize.y);
    }
    else
    {
        textureSize.x = sprites[0].w;
        textureSize.y = sprites[0].h;
    }
    
    return textureSize;
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
            if (currentSprite > currentAnimation.end)
            {
                currentSprite = currentAnimation.start;
            }
        }
        else if (currentAnimation.direction == ANIMATION_REVERSE)
        {
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