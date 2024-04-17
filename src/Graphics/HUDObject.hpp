/**
 * @file HUDObject.hpp
 */

#pragma once

#include <cstdint>
#include <string>

#include "Graphics/Sprite.hpp"
#include "Math/Vector2f.hpp"
#include "Math/Vector2i.hpp"

class HUDObject
{
public:
    /*Internal functions start here*/

    void updateInternal();

    /*Internal functions end here*/


    /**
     * @brief Default cunstructor.
     * 
     */
    HUDObject();

    /**
     * @brief Set the animation of the HUD object.
     * 
     * @param animationName the name of the animation
     */
    void setAnimation(const std::string& animationName);

    /**
     * @brief Set the sprite of the HUD object.
     * 
     * @param spriteName the name of the sprite
     */
    void setSprite(const std::string& spriteName);

    /**
     * @brief Get the texture size of the HUD object.
     * 
     * @return the size of the HUD object in pixels.
     */
    Vector2i getSize() const;

    /**
     * @brief Check if the cursor is on the HUD object.
     * 
     * @return true if the cursor is on the HUD object, false otherwise.
     */
    bool isCursorOnMe() const;

    /**
     * @brief Set the position of the HUD object.
     * 
     * @param x the x position of the HUD object in screen coordinates
     * @param y the y position of the HUD object in screen coordinates
     */
    void setPosition(int x, int y);

    /**
     * @brief Set the position of the HUD object.
     * 
     * @param position the position of the HUD object in screen coordinates
     */
    void setPosition(const Vector2i& position);

    /**
     * @brief Set the size of the HUD object.
     * 
     * @param scale the scale of the HUD object
     */
    void setSize(float scale);

    /**
     * @brief Set the size of the HUD object.
     * 
     * @param width the width of the HUD object in pixels
     * @param height the height of the HUD object in pixels
     */
    void setSize(int width, int height);

    /**
     * @brief Set the size of the HUD object.
     * 
     * @param scale the size of the HUD object in pixels
     */
    void setSize(const Vector2i& scale);

    /**
     * @brief Set the font of the HUD object.
     * 
     * @param fontName the name of the font
     * @param size the size of the font in pixels
     */
    void setFont(const std::string& fontName, int size);
    
    /**
     * @brief Set the text and color of the HUD object.
     * 
     * @param text the text of the HUD object
     * @param red the red value from 0 - 255
     * @param green the green value from 0 - 255
     * @param blue the blue value from 0 - 255
     * @param alpha the alpha value from 0 - 255
     */
    void setText(const std::string& text, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

    /**
     * @brief The update function has to be implemented in every inhereting class. This function is called once every frame.
     * 
     */
    virtual void update();

    /**
     * @brief The destructor can be implemented in inhereting classes.
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