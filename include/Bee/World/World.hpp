#pragma once

#include <vector>

#include "Bee/Entity.hpp"
#include "Bee/Collision/Intersection.hpp"
#include "Bee/Graphics/HUDObject.hpp"
#include "Bee/World/Tiles.hpp"
#include "Bee/World/WorldObject.hpp"

class World
{
public:
    World();
    void addEntity(Entity* entity);
    void removeEntity(Entity* entity);
    void addHUDObject(HUDObject* hudObject);
    void removeHUDObject(HUDObject* hudObject);
    void loadTilemap(std::string tilemapName);
    std::string getTileType(const Vector2f& position) const;
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