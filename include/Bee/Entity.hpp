/**
 * @file Entity.hpp
 */

#pragma once

#include <string>

#include "Bee/Collision/Intersection.hpp"
#include "Graphics/Color.hpp"
#include "Bee/Math/Vector2f.hpp"
#include "Bee/Math/Vector3f.hpp"

class Sprite;

class Entity
{
public:
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
     * @return all intersections of the entity.
     */
    std::vector<Intersection> getIntersections() const;

    /**
     * @brief Get the name of the entity.
     * 
     * @return the name of the entity.
     */
    std::string getName() const;

    /**
     * @brief Get the position of the entity.
     * 
     * @return the position of this entity in world coordinates.
     */
    Vector3f getPosition() const;

    /**
     * @brief Get the rotation of the entity.
     * 
     * @return the rotation of the entity in degrees.
     */
    float getRotation() const;

    Hitbox getHitBox() const;

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

    void setShader(const std::string& shader) const;

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

    void setPosition(const Vector2f& position);

    /**
     * @brief Set the position of the entity.
     * 
     * @param position the position of the entity in world coordinates
     */
    void setPosition(const Vector3f& position);

    void setPositionZ(float z);

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
     * @param size the size of the entity. 
     */
    void setScale(const Vector2f& size);

    /**
     * @brief Set the text and color of the entity.
     * 
     * @param text the text of the entity
     * @param font the font of the text
     * @param fontSize
     * @param color
     */
    void setText(const std::string& text, const std::string& font, int fontSize, const Color& color);

    /**
     * @brief Set the hitbox size of the entity.
     * 
     * @param scale the scale of the hitbox
     */
    void setHitboxScale(float scale);

    /**
     * @brief Set the hitbox size of the entity.
     * 
     * @param scale the size of the hitbox
     */
    void setHitboxScale(const Vector2f& scale);

    /**
     * @brief The update function can be implemented in inheriting classes and is called once every frame.
     * 
     */
    virtual void update();

    virtual void onDraw();

    /**
     * @brief The destructor can be implemented in inheriting classes.
     * 
     */
    virtual ~Entity();

private:
    float rotation = 0;
    Sprite* sprite = nullptr;
    Vector3f position;
    Vector2f rotationCenter = {0.5f, 0.5f};
    Vector2f scale = {1.0f, 1.0f};
    Vector2f hitboxScale = {1.0f, 1.0f};
};