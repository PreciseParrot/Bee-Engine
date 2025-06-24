/**
 * @file HUDObject.hpp
 */

#pragma once

#include <string>

#include "Bee/Graphics/Color.hpp"
#include "Bee/Math/Vector2f.hpp"
#include "Bee/Math/Vector2i.hpp"
#include "Bee/Math/Vector3f.hpp"

class Sprite;

class HUDObject
{
public:
    /**
     * @brief Default constructor.
     * 
     */
    HUDObject();

    Vector3f getPosition() const;

    /**
     * @brief Get the texture size of the HUD object.
     * 
     * @return the size of the HUD object in pixels.
     */
    Vector2i getSize() const;

    /**
     * @brief Get the texture size of the HUD object.
     * 
     * @return the size of the HUD object texture in pixels.
     */
    Vector2i getTextureSize() const;

    /**
     * @brief Check if the cursor is on the HUD object.
     * 
     * @return true if the cursor is on the HUD object, false otherwise.
     */
    bool isCursorOnMe() const;

    void setPosition(const Vector2f& position);

    /**
     * @brief Set the position of the HUD object.
     * 
     * @param position the position of the HUD object in screen coordinates
     */
    void setPosition(const Vector3f& position);

    void setPositionZ(float z);

    /**
     * @brief Set the animation of the HUD object.
     * 
     * @param animationName the name of the animation
     */
    void setAnimation(const std::string& animationName) const;

    /**
     * @brief Set the size of the HUD object.
     * 
     * @param scale the scale of the HUD object
     */
    void setScale(float scale);

    /**
     * @brief Set the size of the HUD object.
     * 
     * @param scale the size of the HUD object in pixels
     */
    void setScale(const Vector2i& scale);

    void setShader(const std::string& shader) const;

    /**
     * @brief Set the sprite of the HUD object.
     * 
     * @param spriteName the name of the sprite
     */
    void setSprite(const std::string& spriteName);

    //void onDraw(void(*func)()) const;
    
    /**
     * @brief Set the text and color of the HUDObject.
     * 
     * @param text the text of the entity
     * @param font the font of the text
     * @param fontSize
     * @param color
     */
    void setText(const std::string& text, const std::string& font, int fontSize, const Color& color);

    /**
     * @brief The update function has to be implemented in every inheriting class. This function is called once every frame.
     * 
     */
    virtual void update();

    virtual void onDraw();

    /**
     * @brief The destructor can be implemented in inheriting classes.
     * 
     */
    virtual ~HUDObject();

private:
    float rotation = 0;
    std::string name;
    Sprite* sprite = nullptr;
    Vector2f rotationCenter = {0.5f, 0.5f};
    Vector3f position;
    Vector2i scale;
};