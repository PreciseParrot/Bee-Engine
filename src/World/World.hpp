/**
 * @file World.hpp
 */

#pragma once

#include <vector>

#include "Entity.hpp"
#include "Collision/Intersection.hpp"
#include "Graphics/HUDObject.hpp"
#include "World/Tiles.hpp"
#include "World/WorldObject.hpp"

class World
{
public:
    /*Internal functions start here*/

    bool isInitialized();
    void initInternal();
    void updateInternal();

    /*Internal functions end here*/

    /**
     * @brief Default constructor.
     * 
     */
    World();

    /**
     * @brief Add an entity to the world.
     * 
     * @param entity the pointer to the entity
     */
    void addEntity(Entity* entity);

    /**
     * @brief Remove an entity from the world.
     * 
     * @param entity the pointer to the entity
     * @return the pointer to the entity if the entity could be removed, NULL otherwise.
     */
    Entity* removeEntity(Entity* entity);

    /**
     * @brief Removes all entities from the world and the deletes them.
     * 
     */
    void deleteAllEntities();

    /**
     * @brief Add a HUD object to the world.
     * 
     * @param hudObject the pointer to the HUD object
     */
    void addHUDObject(HUDObject* hudObject);

    /**
     * @brief Remove a HUD object from the world.
     * 
     * @param hudObject the pointer to the HUD object
     * @return the pointer to the HUD object if the HUD object could be removed, NULL otherwise. 
     */
    HUDObject* removeHUDObject(HUDObject* hudObject);

    /**
     * @brief Removes all HUD objects from the world and deletes them.
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
     * @brief Get all intersections with entities and world objects of an entity.
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
     * @brief The destructor can to be implemented in inhereting classes.
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
    void loadTileset(const std::string source, int firstId);
};