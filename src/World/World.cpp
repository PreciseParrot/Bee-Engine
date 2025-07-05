#include "Bee/World/World.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <tinyxml2.h>

#include "Bee/Bee.hpp"
#include "Bee/Log.hpp"
#include "Bee/Collision/Intersection.hpp"
#include "Bee/Math/Vector3f.hpp"
#include "Tiles.hpp"
#include "Collision/Collision.hpp"
#include "Graphics/Renderer-Internal.hpp"

World::World() = default;

void World::update()
{
    for (Tile &tile : tiles)
    {
        if (tile.animated && tile.animationFrames[tile.animationIndex].duration + tile.frameStartTime <= Bee::getTime())
        {
            tile.frameStartTime = Bee::getTime();
            tile.animationIndex++;
            if (tile.animationIndex >= tile.animationFrames.size())
            {
                tile.animationIndex = 0;
            }
            tile.position.x = tile.animationFrames[tile.animationIndex].tileId % tile.columns * tile.size.x;
            tile.position.y = tile.animationFrames[tile.animationIndex].tileId / tile.columns * tile.size.y;
        }
    }

    Vector2i renderPosition = Renderer::getCameraPosition() - Renderer::getViewPortSize() / 2.0f - Vector2i(1, 1);;
    renderPosition.x = (renderPosition.x > 0) ? renderPosition.x : 0;
    renderPosition.y = (renderPosition.y > 0) ? renderPosition.y : 0;

    Vector2i renderSize = renderPosition + Renderer::getViewPortSize() + Vector2i(3, 3);
    renderSize.x = renderSize.x < worldWidth ? renderSize.x : worldWidth;
    renderSize.y = renderSize.y < worldHeight ? renderSize.y : worldHeight;

    for (size_t i = 0; i < layers.size(); i++)
    {
        for (int x = renderPosition.x; x < renderSize.x; x++)
        {
            for (int y = renderPosition.y; y < renderSize.y; y++)
            {
                const int tileId = layers.at(i).tileIds[x + y * worldWidth];
                if (tileId == 0) continue;

                Vector3f pos;
                pos.x = x;
                pos.y = y;
                pos.z = static_cast<float>(i) - nullLayer + 1;

                Rect rect;
                rect.x = tiles[tileId].position.x;
                rect.y = tiles[tileId].position.y;
                rect.w = tiles[tileId].size.x;
                rect.h = tiles[tileId].size.y;

                Renderer::queueTile(pos, tiles[tileId].textureID, rect);
            }
        }
    }

    for (Entity* entity : entities)
    {
        entity->update();
        entity->BaseObject::update();
        entity->Entity::update();
    }

    for (HUDObject* hudObject : hudObjects)
    {
        hudObject->update();
        hudObject->HUDObject::update();
    }
}

void World::addEntity(Entity* entity)
{
    if (std::ranges::count(entities, entity))
    {
        Log::write("World", LogLevel::warning, "Entity is already present in the world");
    }
    else
    {
        entities.push_back(entity);
    }
}

Entity* World::getEntityByName(const std::string& name) const
{
    // for (Entity* entity : entities)
    // {
    //     if (entity->getName() == name)
    //         return entity;
    // }

    return nullptr;
}

std::vector<Entity*> World::getAllEntities()
{
    return entities;
}

Entity* World::removeEntity(Entity* entity)
{
    if (std::ranges::count(entities, entity))
    {
        std::erase(entities, entity);
        return entity;
    }

    return nullptr;
}

void World::deleteAllEntities()
{
    for (const Entity* entity : entities)
    {
        delete entity;
    }

    entities.clear();
}

void World::addHUDObject(HUDObject* hudObject)
{
    if (std::ranges::count(hudObjects, hudObject))
    {
        Log::write("World", LogLevel::warning, "HUD Object is already present in the world");
    }
    else
    {
        hudObjects.push_back(hudObject);
    }
}

std::vector<HUDObject*> World::getAllHUDObjects()
{
    return hudObjects;
}

HUDObject* World::removeHUDObject(HUDObject* hudObject)
{
    if (std::ranges::count(hudObjects, hudObject))
    {
        std::erase(hudObjects, hudObject);
        return hudObject;
    }

    return nullptr;
}

void World::deleteAllHUDObjects()
{
    for (const HUDObject* hudObject : hudObjects)
    {
        delete hudObject;
    }

    hudObjects.clear();
}

std::vector<WorldObject*> World::getAllWorldObjects() const
{
    return worldObjects;
}

const Properties& World::getTileProperties(const Vector2f& position) const
{
    if (static_cast<int>(position.x) < 0) return tiles[0].properties;
    if (static_cast<int>(position.y) < 0) return tiles[0].properties;
    if (static_cast<int>(position.x) > worldWidth) return tiles[0].properties;
    if (static_cast<int>(position.y) > worldHeight) return tiles[0].properties;

    int tileID = 0;

    for (const TileLayer& layer : layers)
    {
        if (const int tileIDTemp = layer.tileIds[static_cast<int>(position.x) + static_cast<int>(position.y) * worldWidth]; tileIDTemp != 0)
            tileID = tileIDTemp;
    }

    return tiles[tileID].properties;
}

std::vector<Intersection> World::getIntersections(const Entity* entity) const
{
    std::vector<Intersection> intersections;

    for (Entity* entityLoop : entities)
    {
        if (entity == entityLoop) continue;

        Intersection intersection;
        intersection.entity = entityLoop;
        intersection.worldObject = nullptr;
        if (Collision::checkCollision(entity->getHitBox(), entityLoop->getHitBox(), intersection))
        {
            intersections.push_back(intersection);
        }
    }

    for (WorldObject* worldObject : worldObjects)
    {
        Intersection intersection;
        intersection.entity = nullptr;
        intersection.worldObject = worldObject;
        if (Collision::checkCollision(entity->getHitBox(), worldObject->getHitbox(), intersection))
        {
            intersections.push_back(intersection);
        }
    }

    return intersections;
}

void World::loadTileset(const std::string &source, int firstId)
{
    const std::string tileSetPath = "./assets/Worlds/" + source;

    tinyxml2::XMLDocument tilesetXML;
    tilesetXML.LoadFile(tileSetPath.c_str());
    if (tilesetXML.Error())
    {
        Log::write("World", LogLevel::error, "Can't not load tileset: %s / %s", source.c_str(), tilesetXML.ErrorName());
        return;
    }

    tinyxml2::XMLElement* tilesetXMLElement = tilesetXML.FirstChildElement("tileset");
    tinyxml2::XMLElement* imageXMLElement = tilesetXMLElement->FirstChildElement("image");

    int width = tilesetXMLElement->IntAttribute("tilewidth");
    int height = tilesetXMLElement->IntAttribute("tileheight");
    int columns = tilesetXMLElement->IntAttribute("columns");
    int tileCount = tilesetXMLElement->IntAttribute("tilecount");
    std::filesystem::path tilesetTexturePath = imageXMLElement->Attribute("source");
    int textureID = Renderer::loadTexture(tilesetTexturePath.replace_extension().string(), "./assets/Worlds/Tilesets/" + tilesetTexturePath.replace_extension().string() + ".png");

    for (int id = 0; id < tileCount; id++)
    {
        Tile tile;
        tile.animated = false;
        tile.animationIndex = 0;
        tile.frameStartTime = 0;
        tile.columns = columns;
        tile.size.x = width;
        tile.size.y = height;
        tile.tilesetSize.x = imageXMLElement->IntAttribute("width");
        tile.tilesetSize.y = imageXMLElement->IntAttribute("height");
        tile.textureID = textureID;
        tile.position.x = id % tile.columns * tile.size.x;
        tile.position.y = id / tile.columns * tile.size.y;

        tiles.insert(tiles.begin() + id + firstId, tile);
    }

    for (const tinyxml2::XMLElement* tileXMLElement = tilesetXMLElement->FirstChildElement("tile"); tileXMLElement != nullptr; tileXMLElement = tileXMLElement->NextSiblingElement("tile"))
    {
        int id = tileXMLElement->IntAttribute("id");

        if (const tinyxml2::XMLElement* propertiesXMLElement = tileXMLElement->FirstChildElement("properties"))
        {
            for (const tinyxml2::XMLElement* propertyXMLElement = propertiesXMLElement->FirstChildElement("property"); propertyXMLElement != nullptr; propertyXMLElement = propertyXMLElement->NextSiblingElement("property"))
            {
                const char* propertyName = propertyXMLElement->Attribute("name");
                const char* propertyType = propertyXMLElement->Attribute("type");
                const char* propertyValue = propertyXMLElement->Attribute("value");
    
                if (!propertyType)
                {
                    tiles[id + firstId].properties.setString(propertyName, propertyValue);
                    break;
                }
    
                if (!strcmp(propertyType, "bool"))
                {
                    if (!strcmp(propertyValue, "true"))
                    {
                        tiles[id + firstId].properties.setBool(propertyName, true);
                    }
                    else
                    {
                        tiles[id + firstId].properties.setBool(propertyName, false);
                    }
                }
                else if (!strcmp(propertyType, "float"))
                {
                    tiles[id + firstId].properties.setFloat(propertyName, std::stof(propertyValue));
                }
                else if (!strcmp(propertyType, "int"))
                {
                    tiles[id + firstId].properties.setInt(propertyName, std::stoi(propertyValue));
                }
                else
                {
                    tiles[id + firstId].properties.setString(propertyName, propertyValue);
                }
            }
        }

        if (const tinyxml2::XMLElement* animationXML = tileXMLElement->FirstChildElement("animation"))
        {
            for (const tinyxml2::XMLElement* animElement = animationXML->FirstChildElement("frame"); animElement != nullptr; animElement = animElement->NextSiblingElement("frame"))
            {
                tiles[id + firstId].animated = true;

                AnimationTileFrame frame{};
                frame.duration = animElement->IntAttribute("duration");
                frame.tileId = animElement->IntAttribute("tileid");
                tiles[id + firstId].animationFrames.push_back(frame);
            }
        }
    }
    Log::write("World", LogLevel::info, "Loaded %s tileset", tilesetTexturePath.replace_extension().string().c_str());
}

void World::loadTilemap(const std::string& tilemapName)
{
    const std::string tileMapPath = "./assets/Worlds/" + tilemapName + ".tmx";

    tiles.clear();
    layers.clear();
    foregroundLayers.clear();
    worldObjects.clear();

    tinyxml2::XMLDocument tilemapXML;
    tilemapXML.LoadFile(tileMapPath.c_str());
    if (tilemapXML.Error())
    {
        Log::write("World", LogLevel::error, "Can't load tilemap: %s / %s", tilemapName.c_str(), tilemapXML.ErrorName());
        return;
    }

    tinyxml2::XMLElement* mapXMLElement = tilemapXML.FirstChildElement("map");
    worldWidth = mapXMLElement->IntAttribute("width");
    worldHeight = mapXMLElement->IntAttribute("height");
    int tileWidth = mapXMLElement->IntAttribute("tilewidth");
    int tileHeight = mapXMLElement->IntAttribute("tileheight");

    for (const tinyxml2::XMLElement* element = mapXMLElement->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
    {
        const char* layerType = element->Name();
        const char* layerName = element->Attribute("name");

        if (layerName && !strcmp("Entities", layerName))
            nullLayer = layers.size();

        if (!strcmp(layerType, "layer"))
        {
            TileLayer layer;
            layer.name = element->Attribute("name");

            const char* layerDataStr = element->FirstChildElement("data")->GetText();
            std::stringstream layerData(layerDataStr);

            while (layerData.good())
            {
                std::string substr;
                getline(layerData, substr, ',');
                layer.tileIds.push_back(stoi(substr));
            }
            layers.push_back(layer);
        }
        else if (!strcmp(layerType, "objectgroup"))
        {
            for (const tinyxml2::XMLElement* object = element->FirstChildElement("object"); object != nullptr; object = object->NextSiblingElement())
            {
                WorldObject* worldObject = new WorldObject;
                const char* name = object->Attribute("name");
                const char* type = object->Attribute("type");
                float x = object->FloatAttribute("x") / tileWidth;
                float y = object->FloatAttribute("y") / tileHeight;
                float width = object->FloatAttribute("width") / tileWidth;
                float height = object->FloatAttribute("height") / tileHeight;
    
                if (name) worldObject->properties.setString("name", name);
                if (type) worldObject->properties.setString("type", type);
    
                if (const tinyxml2::XMLElement* properties = object->FirstChildElement("properties"))
                {
                    for (const tinyxml2::XMLElement* property = properties->FirstChildElement("property"); property != nullptr; property = property->NextSiblingElement("property"))
                    {
                        const char* propertyName = property->Attribute("name");
                        const char* propertyType = property->Attribute("type");
                        const char* propertyValue = property->Attribute("value");
    
                        if (!propertyType)
                        {
                            worldObject->properties.setString(propertyName, propertyValue);
                            break;
                        }
    
                        if (!strcmp(propertyType, "bool"))
                        {
                            if (!strcmp(propertyValue, "true"))
                            {
                                worldObject->properties.setBool(propertyName, true);
                            }
                            else
                            {
                                worldObject->properties.setBool(propertyName, false);
                            }
                        }
                        else if (!strcmp(propertyType, "float"))
                        {
                            worldObject->properties.setFloat(propertyName, std::stof(propertyValue));
                        }
                        else if (!strcmp(propertyType, "int"))
                        {
                            worldObject->properties.setInt(propertyName, std::stoi(propertyValue));
                        }
                        else
                        {
                            worldObject->properties.setString(propertyName, propertyValue);
                        }
                    }
                }

                Hitbox hitbox;
                const tinyxml2::XMLElement* polygon = object->FirstChildElement("polygon");
                const tinyxml2::XMLElement* ellipse = object->FirstChildElement("ellipse");
                const tinyxml2::XMLElement* point = object->FirstChildElement("point");
    
                if (polygon)
                {
                    std::vector<Vector2f> polygonPoints;
                    std::stringstream polygonPointsStr(polygon->Attribute("points"));
    
                    while (polygonPointsStr.good())
                    {
                        Vector2f polygonPoint;
                        std::string substr;
    
                        getline(polygonPointsStr, substr, ',');
                        polygonPoint.x = stof(substr) / tileWidth;
                        getline(polygonPointsStr, substr, ' ');
                        polygonPoint.y = stof(substr) / tileHeight;
                        polygonPoints.push_back(polygonPoint);
                    }
    
                    for (const Vector2f& polygonPoint : polygonPoints)
                    {
                        hitbox.vertices.push_back(polygonPoint + Vector2f(x, y));
                    }
                }
                else if (ellipse)
                {
                    hitbox.isEllipse = true;
                    hitbox.center.x = x + width / 2;
                    hitbox.center.y = y + height / 2;
                    hitbox.ellipse.x = width / 2;
                    hitbox.ellipse.y = height / 2;
                }
                else if (point)
                {
                    hitbox.center = {x, y};
                    hitbox.vertices.emplace_back(x, y);
                }
                else
                {
                    hitbox.vertices.emplace_back(x, y);
                    hitbox.vertices.emplace_back(x, y + height);
                    hitbox.vertices.emplace_back(x + width, y);
                    hitbox.vertices.emplace_back(x + width, y + height);
                }
                worldObject->setHitbox(hitbox);
                worldObjects.push_back(worldObject);
            }
        }
    }
    
    Tile nullTile;
    nullTile.animated = false;
    nullTile.size.x = 0;
    nullTile.size.y = 0;
    nullTile.position.x = 0;
    nullTile.position.y = 0;
    tiles.push_back(nullTile);

    for (const tinyxml2::XMLElement* element = mapXMLElement->FirstChildElement("tileset"); element != nullptr; element = element->NextSiblingElement("tileset"))
    {
        int firstId = element->IntAttribute("firstgid");
        std::string source = element->Attribute("source");
        loadTileset(source, firstId);
    }

    for (tinyxml2::XMLElement* objectGroup = mapXMLElement->FirstChildElement("objectgroup"); objectGroup != nullptr; objectGroup = objectGroup->NextSiblingElement())
    {
        
    }
    Log::write("World", LogLevel::info, "Loaded %s tilemap", tilemapName.c_str());
}

World::~World()
{
    for (const WorldObject* worldObject : worldObjects)
    {
        delete worldObject;
    }
}