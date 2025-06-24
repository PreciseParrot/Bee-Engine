#include "Sprite.hpp"

#include <fstream>
#include <string>

#include <nlohmann/json.hpp>

#include "Bee/Bee.hpp"
#include "Bee/Log.hpp"
#include "Bee/Math/Vector2f.hpp"
#include "Bee/Math/Vector2i.hpp"
#include "Bee/Math/Vector3f.hpp"
#include "Renderer-Internal.hpp"
#include "Rect.hpp"

Sprite::Sprite() = default;

void Sprite::setShader(const std::string& shader)
{
    shaderID = Renderer::loadShader(shader);
}

void Sprite::setSprite(const std::string& spriteName)
{
    std::string jsonFilePath = "./assets/Sprites/" + spriteName + ".json";
    std::string pngFilePath = "./assets/Sprites/" + spriteName + ".png";

    textureID = Renderer::loadTexture(spriteName, pngFilePath);

    currentAnimation.start = 0;
    currentAnimation.end = 0;
    currentAnimation.direction = AnimationDirection::none;
    animations.insert({"no_animation", currentAnimation});

    std::ifstream jsonFile(jsonFilePath);

    if (jsonFile.fail()) return;

    nlohmann::json spriteData = nlohmann::json::parse(jsonFile);

    for (const nlohmann::json& spriteFrameJson : spriteData["frames"])
    {
        SpriteFrame spriteFrame;
        spriteFrame.x = spriteFrameJson["frame"]["x"].get<int>();
        spriteFrame.y = spriteFrameJson["frame"]["y"].get<int>();
        spriteFrame.w = spriteFrameJson["frame"]["w"].get<int>();
        spriteFrame.h = spriteFrameJson["frame"]["h"].get<int>();
        spriteFrame.duration = spriteFrameJson["duration"].get<int>();

        frames.push_back(spriteFrame);
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

        animations.insert({frameTagJson["name"].get<std::string>(), frameTag});
    }
}

void Sprite::setAnimation(const std::string& animationName)
{
    if (currentAnimationName == animationName) return;

    if (animations.contains(animationName))
    {
        currentAnimation = animations.at(animationName);
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

void Sprite::setText(const std::string& text, const std::string& font, int fontSize, const Color& color)
{
    if (text == "") return;
    
    const SDL_Color sdlColor = {
        static_cast<uint8_t>(color.r * 255),
        static_cast<uint8_t>(color.g * 255),
        static_cast<uint8_t>(color.b * 255),
        static_cast<uint8_t>(color.a * 255)
    };

    Renderer::deleteUniqueTexture(textureID);
    TTF_Font* ttfFont = Renderer::loadFont(font, fontSize);
    SDL_Surface* argbSurface = TTF_RenderUTF8_Blended_Wrapped(ttfFont, text.c_str(), sdlColor, 0);
    SDL_Surface* rgbaSurface = SDL_ConvertSurfaceFormat(argbSurface, SDL_PIXELFORMAT_RGBA32, 0);

    textureID = Renderer::createUniqueTexture(rgbaSurface);

    SDL_FreeSurface(argbSurface);
    SDL_FreeSurface(rgbaSurface);
}

Vector2f Sprite::getTextureSize() const
{
    Vector2f textureSize;
    
    if (frames.empty())
    {
        textureSize = Renderer::getTextureSize(textureID);
    }
    else
    {
        textureSize.x = frames[0].w;
        textureSize.y = frames[0].h;
    }

    return textureSize;
}

void Sprite::updateInternal()
{
    if (frames.empty() || currentAnimation.direction == AnimationDirection::none)
        return;

    if (const uint32_t currentTime = Bee::getTime(); frameStartTime + frames[currentSprite].duration <= currentTime)
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

        switch (currentAnimationDirection)
        {
            case AnimationDirection::forward:
                if (currentSprite > currentAnimation.end)
                {
                    currentSprite = currentAnimation.start;
                }
                break;
            case AnimationDirection::reverse:
                if (currentSprite < currentAnimation.start)
                {
                    currentSprite = currentAnimation.end;
                }
                break;
            case AnimationDirection::pingPong:
                if (currentSprite >= currentAnimation.end && currentAnimationDirection == AnimationDirection::forward)
                {
                    currentAnimationDirection = AnimationDirection::reverse;
                }
                else if (currentSprite <= currentAnimation.start && currentAnimationDirection == AnimationDirection::reverse)
                {
                    currentAnimationDirection = AnimationDirection::forward;
                }
                break;
            case AnimationDirection::reversePingPong:
                
            default:
                break;
        }
    }
}

void Sprite::updateInternalHUD(const Vector3f& position, const Vector2i& scale, const Vector2f& rotationCenter, const float rotation, HUDObject* hudObject)
{
    updateInternal();
    
    Rect rect;

    if (frames.empty())
    {
        Vector2i textureSize = getTextureSize();
        rect.x = 0;
        rect.y = 0;
        rect.w = textureSize.x;
        rect.h = textureSize.y;
    }
    else
    {
        rect.x = frames.at(currentSprite).x;
        rect.y = frames.at(currentSprite).y;
        rect.w = frames.at(currentSprite).w;
        rect.h = frames.at(currentSprite).h;
    }

    Renderer::queueHUD(position, scale, shaderID, textureID, rect, hudObject);
}

void Sprite::updateInternalEntity(const Vector3f& position, const Vector2f& scale, const Vector2f& rotationCenter, const float rotation, Entity* entity)
{
    updateInternal();
    
    Rect rect;

    if (frames.empty())
    {
        Vector2i textureSize = getTextureSize();
        rect.x = 0;
        rect.y = 0;
        rect.w = textureSize.x;
        rect.h = textureSize.y;
    }
    else
    {
        rect.x = frames.at(currentSprite).x;
        rect.y = frames.at(currentSprite).y;
        rect.w = frames.at(currentSprite).w;
        rect.h = frames.at(currentSprite).h;
    }
    
    Renderer::queueEntity(position, scale, shaderID, textureID, rect, entity);
}

Sprite::~Sprite() = default;