#pragma once

#include <vector>

#include "Bee/Entity.hpp"
#include "Bee/Collision/Intersection.hpp"
#include "Bee/Graphics/HUDObject.hpp"
#include "Bee/World/WorldObject.hpp"

struct Tile;
struct TileLayer;

class World
{
public:
    World();
    void addEntity(Entity* entity);
    Entity* removeEntity(Entity* entity);
    void deleteAllEntities();
    void addHUDObject(HUDObject* hudObject);
    HUDObject* removeHUDObject(HUDObject* hudObject);
    void deleteAllHUDObjects();
    void loadTilemap(std::string tilemapName);
    std::string getTileData(const Vector2f& position, std::string index) const;
    std::vector<Intersection> getIntersections(const Entity* entity) const;
    virtual void update();
    virtual void onLoad();
    virtual void onUnLoad();
    virtual ~World();

private:
    int worldHeight;
    int worldWidth; 
    std::vector<Entity*> entities;
    std::vector<WorldObject*> worldObjects;
    std::vector<HUDObject*> hudObjects;
    std::vector<TileLayer> foregroundLayers;
    std::vector<TileLayer> layers;
    std::vector<Tile> tiles;
};