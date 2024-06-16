#include "Properties.hpp"
bool Properties::getBool(const std::string& index) const
{
    return propertiesBool.at(index);
}

float Properties::getFloat(const std::string& index) const
{
    return propertiesFloat.at(index);
}

int Properties::getInt(const std::string& index) const
{
    return propertiesInt.at(index);
}

std::string Properties::getString(const std::string& index) const
{
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
