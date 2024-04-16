/**
 * @file HUDObject.hpp
 */

#pragma once

#include <stdint.h>
#include <string>

#include "Graphics/Sprite.hpp"
#include "Math/Vector2f.hpp"
#include "Math/Vector2i.hpp"

/**
 * @brief The HUDObject class.
 * 
 */
class HUDObject
{
public:
    /*Internal only functions start here*/

    void updateInternal();

    /*Internal only functions end here*/


    /**
     * @brief The constructor function of the HUDObject class.
     * 
     */
    HUDObject();

    /**
     * @brief Set the animation of the HUDObject.
     * 
     * @param animationName the name of the animation
     */
    void setAnimation(const std::string& animationName);

    /**
     * @brief Set the sprite of the HUDObject.
     * 
     * @param spriteName the name of the sprite
     */
    void setSprite(const std::string& spriteName);

    /**
     * @brief Get the texture size of the HUDObject.
     * 
     * @return the size of the HUDObject in pixels.
     */
    Vector2i getSize() const;

    /**
     * @brief Check if the cursor is on the HUDObject.
     * 
     * @return true if the cursor is on the HUDObject, false otherwise.
     */
    bool isCursorOnMe() const;

    /**
     * @brief Set the position of the HUDObject.
     * 
     * @param x the x position of the HUDObject in screen coordinates
     * @param y the y position of the HUDObject in screen coordinates
     */
    void setPosition(int x, int y);

    /**
     * @brief Set the position of the HUDObhect.
     * 
     * @param position the position of the HUDObject in screen coordinates
     */
    void setPosition(const Vector2i& position);

    /**
     * @brief Set the scale of the HUDObject.
     * 
     * @param scale the scale of the HUDObject
     */
    void setScale(float scale);

    /**
     * @brief Set the scale of the HUDObject.
     * 
     * @param width the width of the HUDObject in pixels
     * @param height the height of the HUDObject in pixels
     */
    void setScale(int width, int height);

    /**
     * @brief Set the scale of the HUDObject.
     * 
     * @param scale the size of the HUDObject in pixels
     */
    void setScale(const Vector2i& scale);

    /**
     * @brief Set the font of the HUDObject.
     * 
     * @param fontName the name of the font
     * @param size the size of the font in pixels
     */
    void setFont(const std::string& fontName, int size);
    
    /**
     * @brief Set the text and color of the HUDObject.
     * 
     * @param text the text of the HUDObject
     * @param red the red value from 0 - 255
     * @param green the green value from 0 - 255
     * @param blue the blue value from 0 - 255
     * @param alpha the alpha value from 0 - 255
     */
    void setText(const std::string& text, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

    /**
     * @brief The update function has to be implemented in every inhereting class.
     * 
     */
    virtual void update();

    /**
     * @brief The destructor function can be implemented in inhereting classes.
     * 
     */
    virtual ~HUDObject();

private:
    float rotation = 0;
    std::string name;
    Sprite* sprite = nullptr;
    Vector2f rotationCenter = {0.5f, 0.5f};
    Vector2i position;
    Vector2i scale;
};