#include "Bee/Properties.hpp"

bool Properties::getBool(const std::string& index) const
{
    if (!propertiesBool.contains(index)) return false;
    return propertiesBool.at(index);
}

float Properties::getFloat(const std::string& index) const
{
    if (!propertiesFloat.contains(index)) return 0.0f;
    return propertiesFloat.at(index);
}

int Properties::getInt(const std::string& index) const
{
    if (!propertiesInt.contains(index)) return 0;
    return propertiesInt.at(index);
}

std::string Properties::getString(const std::string& index) const
{
    if (!propertiesString.contains(index)) return "";
    return propertiesString.at(index);
}

void Properties::setBool(const std::string& index, const bool value)
{
    propertiesBool[index] = value;
}

void Properties::setFloat(const std::string& index, const float value)
{
    propertiesFloat[index] = value;
}

void Properties::setInt(const std::string& index, const int value)
{
    propertiesInt[index] = value;
}

void Properties::setString(const std::string& index, const std::string& value)
{
    propertiesString[index] = value;
}
