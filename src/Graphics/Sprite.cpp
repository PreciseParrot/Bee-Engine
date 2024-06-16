#include "Sprite.hpp"

#include <fstream>
#include <string>

#include <nlohmann/json.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Bee.hpp"
#include "Log.hpp"
#include "Renderer.hpp"
#include "Math/Vector2f.hpp"
#include "Math/Vector2i.hpp"

Sprite::Sprite() = default;

void Sprite::setSprite(const std::string& spriteName)
{
    std::string jsonFilePath = "./assets/Sprites/" + spriteName + ".json";
    std::string pngFilePath = "./assets/Sprites/" + spriteName + ".png";

    texture = Renderer::loadTexture(spriteName, pngFilePath);

    currentAnimation.start = 0;
    currentAnimation.end = 0;
    currentAnimation.direction = AnimationDirection::none;
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

        if (std::string direction = frameTagJson["direction"].get<std::string>(); direction == "forward")
        {
            frameTag.direction = AnimationDirection::forward;
        }
        else if (direction == "reverse")
        {
            frameTag.direction = AnimationDirection::reverse;
        }
        else if (direction == "pingpong")
        {
            frameTag.direction = AnimationDirection::pingPong;
        }

        frameTags.insert({frameTagJson["name"].get<std::string>(), frameTag});
    }
}

void Sprite::setAnimation(const std::string& animationName)
{
    if (currentAnimationName == animationName) return;

    if (frameTags.contains(animationName))
    {
        currentAnimation = frameTags.at(animationName);
    }
    else
    {
        Log::write("Sprite", LogLevel::warning, "Animation: %s not found", animationName.c_str());
    }

    if (currentAnimation.direction == AnimationDirection::reverse)
    {
        currentAnimationDirection = AnimationDirection::reverse;
        currentSprite = currentAnimation.end;
    }
    else
    {
        currentAnimationDirection = AnimationDirection::forward;
        currentSprite = currentAnimation.start;
    }
    currentAnimationName = animationName;
}

void Sprite::setFont(const std::string& fontName, const int size)
{
    font = Renderer::loadFont(fontName, size);
}

void Sprite::setText(const std::string& text, const uint8_t red, const uint8_t green, const uint8_t blue, const uint8_t alpha)
{
    if (this->text == text) return;

    this->text = text;
    SDL_DestroyTexture(texture);

    const SDL_Color color = {red, green, blue, alpha};
    SDL_Surface* surface = TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, 0);
    texture = Renderer::createTexture(surface);
    SDL_FreeSurface(surface);
    SDL_SetTextureScaleMode(texture, SDL_ScaleModeBest);
}

Vector2i Sprite::getTextureSize() const
{
    Vector2i textureSize;

    if (sprites.empty())
    {
        SDL_QueryTexture(texture, nullptr, nullptr, &textureSize.x, &textureSize.y);
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
    if (sprites.empty() || currentAnimation.direction == AnimationDirection::none)
        return;

    if (const uint32_t currentTime = Bee::getTime(); frameStartTime + sprites[currentSprite].duration <= currentTime)
    {
        frameStartTime = currentTime;

        if (currentAnimationDirection == AnimationDirection::forward)
        {
            currentSprite++;
        }
        else
        {
            currentSprite--;
        }

        if (currentAnimation.direction == AnimationDirection::forward)
        {
            if (currentSprite > currentAnimation.end)
            {
                currentSprite = currentAnimation.start;
            }
        }
        else if (currentAnimation.direction == AnimationDirection::reverse)
        {
            if (currentSprite < currentAnimation.start)
            {
                currentSprite = currentAnimation.end;
            }
        }
        else if (currentAnimation.direction == AnimationDirection::pingPong)
        {
            if (currentSprite >= currentAnimation.end && currentAnimationDirection == AnimationDirection::forward)
            {
                currentAnimationDirection = AnimationDirection::reverse;
            }
            else if (currentSprite <= currentAnimation.start && currentAnimationDirection == AnimationDirection::reverse)
            {
                currentAnimationDirection = AnimationDirection::forward;
            }
        }
    }
}

void Sprite::updateInternalHUD(const Vector2i& position, const Vector2i& scale, const Vector2f& rotationCenter, const float rotation)
{
    updateInternal();
    Renderer::drawHUD(position, scale, reinterpret_cast<SDL_Rect *>(&sprites[currentSprite]), texture, rotationCenter, rotation);
}

void Sprite::updateInternalEntity(const Vector2f& position, const Vector2f& scale, const Vector2f& rotationCenter, const float rotation)
{
    updateInternal();
    Renderer::drawSprite(position, scale, reinterpret_cast<SDL_Rect *>(&sprites[currentSprite]), texture, rotationCenter, rotation);
}

Sprite::~Sprite() = default;