/**
 * @file Entity.hpp
 */

#pragma once

#include <cstdint>
#include <string>

#include "Properties.hpp"
#include "Collision/Hitbox.hpp"
#include "Collision/Intersection.hpp"
#include "Graphics/Sprite.hpp"
#include "Math/Vector2f.hpp"

class Entity
{
public:
    //Internal functions start here

    void updateInternal() const;
    Hitbox getHitBox() const;

    //Internal functions end here

    /**
     * @brief The custom properties of the entity.
     *
     */
    Properties properties;

    /**
     * @brief Default constructor.
     * 
     */
    Entity();

    /**
     * @brief Get all intersections with entities and world objects with the entity.
     * 
     * @return all intersections of the entitiy.
     */
    std::vector<Intersection> getIntersections() const;

    /**
     * @brief Get the name of the entitiy.
     * 
     * @return the name of the entity.
     */
    std::string getName() const;

    /**
     * @brief Get the position of the entity.
     * 
     * @return the position of this entitiy in world coordinates.
     */
    Vector2f getPosition() const;

    /**
     * @brief Get the rotation of the entity.
     * 
     * @return the rotation of the entity in degrees.
     */
    float getRotation() const;

    /**
     * @brief Move the entity by a given offset.
     * 
     * @param offset the offset to move the entity by.
     */
    void moveOffset(const Vector2f& offset);

    /**
     * @brief Check if the cursor is on the entity.
     * 
     * @return true if the cursor is on the entity, false otherwise. 
     */
    bool isCursorOnMe() const;

    /**
     * @brief Load a sprite.
     * 
     * @param spriteName the name of the sprite.
     */
    void setSprite(const std::string& spriteName);

    /**
     * @brief Set the animation of the entity.
     * 
     * @param animationName the name of the animation.
     */
    void setAnimation(const std::string& animationName) const;

    /**
     * @brief Set the name of the entity.
     * 
     * @param name the name of the entity.
     */
    void setName(const std::string& name);

    /**
     * @brief Set the position of the entity.
     * 
     * @param x the x position of the entity in world coordinates
     * @param y the y position of the entity in world coordinates
     */
    void setPosition(float x, float y);

    /**
     * @brief Set the position of the entity.
     * 
     * @param position the position of the entity in world coordinates
     */
    void setPosition(const Vector2f& position);

    /**
     * @brief Set the rotation of the entity.
     * 
     * @param rotation the rotation of the entity in degrees
     */
    void setRotation(float rotation);

    /**
     * @brief Set the size of the entity.
     * 
     * @param scale the scale of the entity
     */
    void setScale(float scale);

    /**
     * @brief Set the size of the entity.
     * 
     * @param width the width of the entity
     * @param height the height of the entity
     */
    void setScale(float width, float height);

    /**
     * @brief Set the size of the entity.
     * 
     * @param size the size of the entity. 
     */
    void setScale(const Vector2f& size);

    /**
     * @brief Set the font of the entity.
     * 
     * @param fontName the name of the font
     * @param size the size of the font in pixels
     */
    void setFont(const std::string& fontName, int size) const;

    /**
     * @brief Set the text and color of the entity.
     * 
     * @param text the text of the entity
     * @param red the red value from 0 - 255
     * @param green the green value from 0 - 255
     * @param blue the blue value from 0 - 255
     * @param alpha the alpha value from 0 - 255
     */
    void setText(const std::string& text, uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

    /**
     * @brief Set the hitbox size of the entity.
     * 
     * @param scale the scale of the hitbox
     */
    void setHitboxScale(float scale);

    /**
     * @brief Set the hitbox size of the entity.
     * 
     * @param width the width of the hitbox
     * @param height the height of the hitbox
     */
    void setHitboxScale(float width, float height);

    /**
     * @brief Set the hitbox size of the entity.
     * 
     * @param scale the size of the hitbox
     */
    void setHitboxScale(const Vector2f& scale);

    /**
     * @brief The update function can be implemented in inhereting classes. This function is called once every frame.
     * 
     */
    virtual void update();

    /**
     * @brief The destructor can be implemented in inhereting classes.
     * 
     */
    virtual ~Entity();

private:
    float rotation = 0;
    Sprite* sprite = nullptr;
    Vector2f position;
    Vector2f rotationCenter = {0.5f, 0.5f};
    Vector2f scale = {1.0f, 1.0f};
    Vector2f hitboxScale = {1.0f, 1.0f};
};