#pragma once

#include <string>
#include <unordered_map>

class Properties
{
public:
    /**
     * @brief Get a bool property.
     *
     * @param index the name of the property
     * @return the value of the property.
     */
    bool getBool(const std::string& index) const;

    /**
     * @brief Get a float property.
     *
     * @param index the name of the property
     * @return the value of the property.
     */
    float getFloat(const std::string& index) const;

    /**
     * @brief Get an int property.
     *
     * @param index the name of the property
     * @return the value of the property.
     */
    int getInt(const std::string& index) const;

    /**
     * @brief Get a string property.
     *
     * @param index the name of the property
     * @return the value of the property.
     */
    std::string getString(const std::string& index) const;

    /**
     * @brief Set a bool property.
     *
     * @param index the name of the property
     * @param value the value of the property
     */
    void setBool(const std::string& index, bool value);

    /**
     * @brief Set a float property.
     * @param index the name of the property
     * @param value the value of the property
     */
    void setFloat(const std::string& index, float value);

    /**
     * @brief Set an int property.
     *
     * @param index the name of the property
     * @param value the value of the property
     */
    void setInt(const std::string& index, int value);

    /**
     * @brief Set a string property.
     * @param index the name of the property
     * @param value the value of the property
     */
    void setString(const std::string& index, const std::string& value);

private:
    std::unordered_map<std::string, bool> propertiesBool;
    std::unordered_map<std::string, float> propertiesFloat;
    std::unordered_map<std::string, int> propertiesInt;
    std::unordered_map<std::string, std::string> propertiesString;
};
