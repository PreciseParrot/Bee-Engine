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
    //Internal functions start here

    void initInternal();
    void updateInternal();

    //Internal functions end here

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
     * @brief Get an entity in the world by name.
     * 
     * @param name the name of the entity
     * @return the first entity in the world that has the specified name.
     */
    Entity* getEntityByName(const std::string& name);

    /**
     * @brief Get all the entities in the world.
     * 
     * @return all the entities in the world.
     */
    std::vector<Entity*> getAllEntities();

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
     * @brief Get all the HUD objects in the world.
     * 
     * @return all the HUD objects in the world.
     */
    std::vector<HUDObject*> getAllHUDObjects();

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
     * @brief Get all the world objects in the world.
     *
     * @return all the world objects in the world.
     */
    std::vector<WorldObject*> getAllWorldObjects() const;

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
     * @brief The update function can be implemented in inhereting classes. This function is called once every frame.
     * 
     */
    virtual void update() = 0;

    /**
     * @brief The onLoad function can be implemented in inhereting classes. This function is called everytime the world is loaded.
     * 
     */
    virtual void onLoad() = 0;

    /**
     * @brief The onUnload function can be implemented in inhereting classes. This function is called everytime the world is unloaded.
     * 
     */
    virtual void onUnload() = 0;

    /**
     * @brief The destructor can be implemented in inhereting classes.
     * 
     */
    virtual ~World();

private:
    int worldHeight = 0;
    int worldWidth = 0;
    std::vector<Entity*> entities;
    std::vector<WorldObject*> worldObjects;
    std::vector<HUDObject*> hudObjects;
    std::vector<TileLayer> foregroundLayers;
    std::vector<TileLayer> layers;
    std::vector<Tile> tiles;
    void loadTileset(const std::string& source, int firstId);
};