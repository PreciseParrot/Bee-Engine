#pragma once

#include <string>

enum class LogLevel
{
    Info,
    Warning,
    Error
};

namespace Log
{
    void write(const std::string& format, ...);
    void write(const std::string& source, LogLevel level, const std::string& format, ...);
}