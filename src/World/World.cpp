#include "World.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <tinyxml2.h>
#include <nlohmann/json.hpp>

#include "Bee.hpp"
#include "Entity.hpp"
#include "Collision/Collision.hpp"
#include "Collision/Intersection.hpp"
#include "Graphics/Renderer.hpp"

World::World() {}

void World::updateInternal()
{
    for (const TileLayer &layer : layers)
    {
        for (int i = 0; i < roomHeight; i++)
        {
            for (int j = 0; j < roomWidth; j++)
            {
                int tileId = layer.tileIds[j + i * roomWidth];
                if (tileId == 0) continue;

                Vector2i pos;
                pos.x = j;
                pos.y = i;

                SDL_Rect srcRect;
                srcRect.h = tiles[tileId].height;
                srcRect.w = tiles[tileId].width;
                srcRect.x = tiles[tileId].currentX;
                srcRect.y = tiles[tileId].currentY;

                Renderer::draw(pos, &srcRect, tiles[tileId].p_texure);
            }
        }   
    }
    
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
            int row;
            tile.currentX = (tile.animationFrames[tile.animationIndex].tileId % tile.columns) * tile.width;
            tile.currentY = (tile.animationFrames[tile.animationIndex].tileId / tile.columns) * tile.height;
        }
    }

    for (Entity* p_entity : entities)
    {
        p_entity->updateInternal();
        p_entity->update();
    }

    for (const TileLayer &layer : foregroundLayers)
    {
        for (int i = 0; i < roomHeight; i++)
        {
            for (int j = 0; j < roomWidth; j++)
            {
                int tileId = layer.tileIds[j + i * roomWidth];
                if (tileId == 0) continue;

                Vector2i pos;
                pos.x = j;
                pos.y = i;

                SDL_Rect srcRect;
                srcRect.h = tiles[tileId].height;
                srcRect.w = tiles[tileId].width;
                srcRect.x = tiles[tileId].currentX;
                srcRect.y = tiles[tileId].currentY;

                Renderer::draw(pos, &srcRect, tiles[tileId].p_texure);
            }
        }   
    }
}

void World::addEntity(Entity* p_entity)
{
    entities.push_back(p_entity);
}

void World::removeEntity(Entity* p_entity)
{
    entities.erase(std::remove(entities.begin(), entities.end(), p_entity), entities.end());
}

std::string World::getTileType(const Vector2f& position) const
{
    int x = position.x;
    int y = position.y;
    if (x < 0) return "";
    if (y < 0) return "";
    if (x > roomWidth) return "";
    if (y > roomHeight) return "";
    std::string type;
    for (const TileLayer& layer : layers)
    {
        int tileId = layer.tileIds[x + y * roomWidth];
        if (tiles[tileId].type != "")
        {
            type = tiles[tileId].type;
        }
    }
    return type;
}

std::vector<Intersection> World::getIntersections(const Entity* p_entity) const
{
    std::vector<Intersection> intersections;

    for (Entity* p_entityLoop : entities)
    {
        if (p_entity == p_entityLoop) continue;

        Intersection intersection;
        intersection.p_entity = p_entityLoop;
        intersection.p_worldObject = NULL;
        if (Collision::checkCollision(p_entity->getHitBox(), p_entityLoop->getHitBox(), &intersection))
            intersections.push_back(intersection);
    }

    for (WorldObject* p_worldObject : worldObjects)
    {
        Intersection intersection;
        intersection.p_entity = NULL;
        intersection.p_worldObject = p_worldObject;
        if (Collision::checkCollision(p_entity->getHitBox(), p_worldObject->hitBox, &intersection))
            intersections.push_back(intersection);
    }

    return intersections;
}

void World::loadTileset(const std::string source, int firstId)
{
    const std::string tileSetPath = "./assets/Worlds/" + source;

    tinyxml2::XMLDocument tilesetXML;
    tilesetXML.LoadFile(tileSetPath.c_str());
    if (tilesetXML.Error())
    {
        std::cout << "Could not load tileset: " << tilesetXML.ErrorName() << std::endl;
        return;
    }

    tinyxml2::XMLElement* tilesetXMLElement = tilesetXML.FirstChildElement("tileset");
    tinyxml2::XMLElement* imageXMLElement = tilesetXMLElement->FirstChildElement("image");

    int width = tilesetXMLElement->IntAttribute("tilewidth");
    int height = tilesetXMLElement->IntAttribute("tileheight");
    int columns = tilesetXMLElement->IntAttribute("columns");
    int tileCount = tilesetXMLElement->IntAttribute("tilecount");
    std::filesystem::path tilesetTexture = imageXMLElement->Attribute("source");
    SDL_Texture* p_texture = Renderer::loadTexture(tilesetTexture.replace_extension().string(), "./assets/Worlds/Tilesets/" + tilesetTexture.string());

    for (int id = 0; id < tileCount; id++)
    {
        Tile tile;
        tile.animated = false;
        tile.animationIndex = 0;
        tile.frameStartTime = 0;
        tile.columns = columns;
        tile.width = width;
        tile.height = height;
        tile.tilesetWidth = imageXMLElement->IntAttribute("width");
        tile.tilesetHeight = imageXMLElement->IntAttribute("height");
        tile.p_texure = p_texture;
        tile.x = (id % tile.columns) * tile.width;
        tile.y = (id / tile.columns) * tile.height;
        tile.currentX = tile.x;
        tile.currentY = tile.y;
        
        tiles.insert(tiles.begin() + id + firstId, tile);
    }

    for (const tinyxml2::XMLElement* element = tilesetXMLElement->FirstChildElement("tile"); element != NULL; element = element->NextSiblingElement("tile"))
    {
        int id = element->IntAttribute("id");
        const char* tileType = element->Attribute("type");
        if (tileType) tiles[id + firstId].type = element->Attribute("type");

        const tinyxml2::XMLElement* animationXML = element->FirstChildElement("animation");
        if (animationXML)
        {
            for (const tinyxml2::XMLElement* animElement = animationXML->FirstChildElement("frame"); animElement != NULL; animElement = animElement->NextSiblingElement("frame"))
            {
                tiles[id + firstId].animated = true;

                AnimationTileFrame frame;
                frame.duration = animElement->IntAttribute("duration");
                frame.tileId = animElement->IntAttribute("tileid");
                tiles[id + firstId].animationFrames.push_back(frame);
            }
        }
    }
}

void World::loadTilemap(const std::string tilemapName)
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
        std::cout << "Could not load tilemap: " << tilemapXML.ErrorName() << std::endl;
        return;
    }

    tinyxml2::XMLElement* mapXMLElement = tilemapXML.FirstChildElement("map");
    roomWidth = mapXMLElement->IntAttribute("width");
    roomHeight = mapXMLElement->IntAttribute("height");
    float tileWidth = mapXMLElement->IntAttribute("tilewidth");
    float tileHeight = mapXMLElement->IntAttribute("tileheight");

    for (const tinyxml2::XMLElement* element = mapXMLElement->FirstChildElement("layer"); element != NULL; element = element->NextSiblingElement("layer"))
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

        const char* layerClass = element->Attribute("class");
        if (layerClass && !strcmp("foreground", layerClass))
        {
            foregroundLayers.push_back(layer);
        }
        else
        {
            layers.push_back(layer);
        }
    }

    Tile nullTile;
    nullTile.type = "";
    nullTile.animated = false;
    nullTile.height = 0;
    nullTile.width = 0;
    nullTile.x = 0;
    nullTile.y = 0;
    nullTile.currentX = 0;
    nullTile.currentY = 0;
    tiles.push_back(nullTile);

    for (const tinyxml2::XMLElement* element = mapXMLElement->FirstChildElement("tileset"); element != NULL; element = element->NextSiblingElement("tileset"))
    {
        int firstId = element->IntAttribute("firstgid");
        std::string source = element->Attribute("source");
        loadTileset(source, firstId);
    }

    for (tinyxml2::XMLElement* objectGroup = mapXMLElement->FirstChildElement("objectgroup"); objectGroup != NULL; objectGroup = objectGroup->NextSiblingElement())
    {
        for (tinyxml2::XMLElement* object = objectGroup->FirstChildElement("object"); object != NULL; object = object->NextSiblingElement())
        {
            WorldObject* p_worldObject = new WorldObject;
            const char* name = object->Attribute("name");
            const char* type = object->Attribute("type");
            float x = object->FloatAttribute("x") / tileWidth;
            float y = object->FloatAttribute("y") / tileHeight;
            float width = object->FloatAttribute("width") / tileWidth;
            float height = object->FloatAttribute("height") / tileHeight;

            if (name) p_worldObject->name = name;
            if (type) p_worldObject->type = type;

            tinyxml2::XMLElement* polygon = object->FirstChildElement("polygon");
            tinyxml2::XMLElement* ellipse = object->FirstChildElement("ellipse");
            tinyxml2::XMLElement* point = object->FirstChildElement("point");
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
                    p_worldObject->hitBox.hitBoxPoints.push_back(polygonPoint + Vector2f(x, y));
                }
            }
            else if (ellipse)
            {

            }
            else if (point)
            {

            }
            else
            {
                p_worldObject->hitBox.hitBoxPoints.push_back(Vector2f(x, y));
                p_worldObject->hitBox.hitBoxPoints.push_back(Vector2f(x, y + height));
                p_worldObject->hitBox.hitBoxPoints.push_back(Vector2f(x + width, y));
                p_worldObject->hitBox.hitBoxPoints.push_back(Vector2f(x + width, y + height));
            }
            worldObjects.push_back(p_worldObject);
        }
    }
}

void World::update() {}

void World::onLoad() {}

void World::onUnLoad() {}

World::~World()
{
    for (Entity* p_entity : entities)
    {
        delete p_entity;
    }
}