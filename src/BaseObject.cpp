#include "Bee/BaseObject.hpp"

#include <fstream>
#include <string>

#include <nlohmann/json.hpp>

#include "Bee/Bee.hpp"
#include "Bee/Log.hpp"
#include "Graphics/Renderer-Internal.hpp"

void BaseObject::setShader(const std::string& shader)
{
    shaderID = Renderer::loadShader(shader);
}

void BaseObject::setSprite(const std::string& spriteName)
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

void BaseObject::setAnimation(const std::string& animationName)
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
    frameStartTime = Bee::getTime();
}

void BaseObject::setText(const std::string& text, const std::string& font, int fontSize, const Color& color)
{
    if (text == "") return;
    
    const SDL_Color sdlColor =
    {
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

void BaseObject::setScale(const Vector2f& scale)
{
    this->scale = scale;
}

void BaseObject::setHitboxScale(const float scale)
{
    const Vector2f textureSize = getTextureSize();
    this->hitboxScale.x = textureSize.x / textureSize.y * scale;
    this->hitboxScale.y = scale;
}

void BaseObject::setHitboxScale(const Vector2f& scale)
{
    hitboxScale = scale;
}

void BaseObject::setPosition(const Vector2f& position)
{
    this->position.x = position.x;
    this->position.y = position.y;
}

void BaseObject::setPosition(const Vector3f& position)
{
    this->position = position;
}

void BaseObject::setPositionZ(float z)
{
    position.z = z;
}

void BaseObject::moveOffset(const Vector2f& offset)
{
    position.x += offset.x;
    position.y += offset.y;
}

Vector3f BaseObject::getPosition() const
{
    return position;
}

Vector2f BaseObject::getScale() const
{
    return scale;
}

Vector2i BaseObject::getTextureSize() const
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

Hitbox BaseObject::getHitBox() const
{
    Hitbox hitbox;
    const float c = cosf(0.0f * std::numbers::pi_v<float> / 180.0f);
    const float s = sinf(0.0f * std::numbers::pi_v<float> / 180.0f);
    
    hitbox.center = position;
    hitbox.vertices.emplace_back(position.x - hitboxScale.x / 2.0f * c - hitboxScale.y / 2.0f * s, position.y - hitboxScale.x / 2.0f * s + hitboxScale.y / 2.0f * c);
    hitbox.vertices.emplace_back(position.x - hitboxScale.x / 2.0f * c + hitboxScale.y / 2.0f * s, position.y - hitboxScale.x / 2.0f * s - hitboxScale.y / 2.0f * c);
    hitbox.vertices.emplace_back(position.x + hitboxScale.x / 2.0f * c - hitboxScale.y / 2.0f * s, position.y + hitboxScale.x / 2.0f * s + hitboxScale.y / 2.0f * c);
    hitbox.vertices.emplace_back(position.x + hitboxScale.x / 2.0f * c + hitboxScale.y / 2.0f * s, position.y + hitboxScale.x / 2.0f * s - hitboxScale.y / 2.0f * c);

    return hitbox;
}

void BaseObject::update()
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

void BaseObject::onDraw()
{
    
}
