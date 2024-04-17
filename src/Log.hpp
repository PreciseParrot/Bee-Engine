/**
 * @file Log.hpp
 */

#pragma once

#include <string>

enum class LogLevel
{
    Info,
    Warning,
    Error
};

/**
 * @namespace Log
 * 
 * @brief All the logger related functions.
 * 
 */
namespace Log
{
    /**
     * @brief Write a simple formatted debug log message
     * 
     * @param format the formatted log message
     * @param ... additional parameters
     */
    void write(const std::string& format, ...);

    /**
     * @brief Write a formatted log message
     * 
     * @param source the origin of the log message
     * @param level the level of the log message
     * @param format the formatted log message
     * @param ... additional parameters
     */
    void write(const std::string& source, LogLevel level, const std::string& format, ...);
}