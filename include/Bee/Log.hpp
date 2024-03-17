#pragma once

#include <string>

enum LogLevel
{
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
};

namespace Log
{
    void write(const std::string& message);
    void write(const std::string& source, LogLevel level, const std::string& message, const std::string& message2 = std::string());
}