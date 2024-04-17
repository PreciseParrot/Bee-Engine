/**
 * @file WorldObject.hpp
 */

#pragma once

#include <string>
#include <unordered_map>

#include "Bee/Collision/Hitbox.hpp"

class WorldObject
{
public:
    /**
     * @brief Get the data of the world object. Use `"name"` to get the name and `"type"` to get the class of the world object.
     * 
     * @param index the name the data variable
     * @return the data of the world object.
     */
    std::string getData(const std::string& index) const;
    
    /**
     * @brief Set the data of the world object.
     * 
     * @param index the name of the data variable
     * @param data the data of the world object
     */
    void setData(const std::string& index, const std::string& data);

private:
    Hitbox hitbox;
    std::unordered_map<std::string, std::string> data;
};