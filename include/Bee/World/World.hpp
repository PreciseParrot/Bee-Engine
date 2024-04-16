/**
 * @file World.hpp
 */

#pragma once

#include <vector>

#include "Bee/Entity.hpp"
#include "Bee/Collision/Intersection.hpp"
#include "Bee/Graphics/HUDObject.hpp"
#include "Bee/World/WorldObject.hpp"

class Tile;
class TileLayer;

class World
{
public:
    /**
     * @brief Construct a new World object
     * 
     */
    World();

    /**
     * @brief Add an Entity to the world.
     * 
     * @param entity the pointer to the entity
     */
    void addEntity(Entity* entity);

    /**
     * @brief Remove an Entity from the world.
     * 
     * @param entity the pointer to the entity
     * @return the pointer to the Entity if the Entity could be removed, NULL otherwise.
     */
    Entity* removeEntity(Entity* entity);

    /**
     * @brief Removes all entities from the world and the deletes them.
     * 
     */
    void deleteAllEntities();

    /**
     * @brief Add a HUDObject to the world.
     * 
     * @param hudObject the pointer to the HUDObject
     */
    void addHUDObject(HUDObject* hudObject);

    /**
     * @brief Remove a HUDObject from the world.
     * 
     * @param hudObject the pointer to the HUDObject
     * @return the pointer to the HUDObject if the HUDObject could be removed, NULL otherwise. 
     */
    HUDObject* removeHUDObject(HUDObject* hudObject);

    /**
     * @brief Removes all HUDObjects from the world and deletes them.
     * 
     */
    void deleteAllHUDObjects();

    /**
     * @brief Load a tilemap.
     * 
     * @param tilemapName the name of the tilemap
     */
    void loadTilemap(const std::string& tilemapName);

    /**
     * @brief Get the data of a tile. Use `"type"` to get the class of the tile.
     * 
     * @param position the position of the tile in world coordinates
     * @param index the name the data variable
     * @return the data of the tile.
     */
    std::string getTileData(const Vector2f& position, const std::string& index) const;

    /**
     * @brief Get all the intersection with entities and world objects of an entity.
     * 
     * @param entity the pointer of the entity
     * @return all intersections of the entity.
     */
    std::vector<Intersection> getIntersections(const Entity* entity) const;

    /**
     * @brief The init function has to be implemented in every inhereting class. This function is called when the world is initialized.
     * 
     */
    virtual void init();

    /**
     * @brief The update function has to be implemented in every inhereting class. This function is called once every frame.
     * 
     */
    virtual void update();

    /**
     * @brief The onLoad function has to be implemented in every inhereting class. This function is called everytime the world is loaded.
     * 
     */
    virtual void onLoad();

    /**
     * @brief The onUnload function has to be implemented in every inhereting class. This function is called everytime the world is unloaded.
     * 
     */
    virtual void onUnLoad();

    /**
     * @brief The destructor function can to be implemented in inhereting classes.
     * 
     */
    virtual ~World();

private:
    int worldHeight = 0;
    int worldWidth = 0; 
    bool initialized = false;
    std::vector<Entity*> entities;
    std::vector<WorldObject*> worldObjects;
    std::vector<HUDObject*> hudObjects;
    std::vector<TileLayer> foregroundLayers;
    std::vector<TileLayer> layers;
    std::vector<Tile> tiles;
};