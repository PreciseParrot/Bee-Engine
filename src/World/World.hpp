#pragma once

#include <vector>

#include "Entity.hpp"
#include "Collision/Intersection.hpp"
#include "World/Tiles.hpp"
#include "World/WorldObject.hpp"

class World
{
public:
    World();
    void updateInternal();
    void addEntity(Entity* p_entity);
    void removeEntity(Entity* p_entity);
    void loadTilemap(std::string tilemapName);
    std::string getTileType(const Vector2f& position) const;
    std::vector<Intersection> getIntersections(const Entity* p_entity) const;
    virtual void update();
    virtual void onLoad();
    virtual void onUnLoad();
    virtual ~World();

private:
    int roomHeight;
    int roomWidth; 
    std::vector<Entity*> entities;
    std::vector<WorldObject*> worldObjects;
    std::vector<TileLayer> foregroundLayers;
    std::vector<TileLayer> layers;
    std::vector<Tile> tiles;
    void loadTileset(const std::string source, int firstId);
};