#include "Log.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

#ifdef _WIN32
    #include <windows.h>
#endif

void Log::write(const std::string& message)
{
    Log::write("Debug", LOG_INFO, message);
}

void Log::write(const std::string& source, LogLevel level, const std::string& message, const std::string& message2)
{
    time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    tm* localTime = std::localtime(&time);
    std::stringstream output;
    
    switch (level)
    {
        case LOG_INFO:
            output << "[" << std::setw(2) << std::setfill('0') << localTime->tm_hour << ":"<< std::setw(2) << std::setfill('0')  << localTime->tm_min << ":"<< std::setw(2) << std::setfill('0')  << localTime->tm_sec << "] [" << source << "] " << "[INFO] " << message << message2 << std::endl;
            break;
        case LOG_WARNING:
            output << "[" << std::setw(2) << std::setfill('0') << localTime->tm_hour << ":"<< std::setw(2) << std::setfill('0')  << localTime->tm_min << ":"<< std::setw(2) << std::setfill('0')  << localTime->tm_sec << "] [" << source << "] " << "[INFO] " << message << message2 << std::endl;
            break;
        case LOG_ERROR:
            output << "[" << std::setw(2) << std::setfill('0') << localTime->tm_hour << ":"<< std::setw(2) << std::setfill('0')  << localTime->tm_min << ":"<< std::setw(2) << std::setfill('0')  << localTime->tm_sec << "] [" << source << "] " << "[INFO] " << message << message2 << std::endl;
            break;
    }

    #ifdef _WIN32
        switch (level)
        {
            case LOG_INFO:
                std::cout << output.str();
                break;
            case LOG_WARNING:
                std::cout << output.str();
                break;
            case LOG_ERROR:
                std::cout << output.str();
                break;
        }
    #elif __linux__
        switch (level)
        {
            case LOG_INFO:
                std::cout << output.str();
                break;
            case LOG_WARNING:
                std::cout << "\033[33m" << output.str() << "\033[0m";
                break;
            case LOG_ERROR:
                std::cout << "\033[31m" << output.str() << "\033[0m";
                break;
        }
    #endif
}