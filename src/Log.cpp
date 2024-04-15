#include "Log.hpp"

#include <stdarg.h>
#include <cstdio>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

static const int bufferSize = 200; 

void Log::write(const std::string& format, ...)
{
    char buffer[bufferSize];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, bufferSize, format.c_str(), args);
    va_end(args);
    Log::write("Debug", LogLevel::Info, buffer);
}

void Log::write(const std::string& source, LogLevel level, const std::string& format, ...)
{
    char buffer[bufferSize];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, bufferSize, format.c_str(), args);
    va_end(args);

    time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    tm* localTime = std::localtime(&time);
    std::stringstream output;
    
    switch (level)
    {
        case LogLevel::Info:
            output << "[" << std::setw(2) << std::setfill('0') << localTime->tm_hour << ":"<< std::setw(2) << std::setfill('0')  << localTime->tm_min << ":"<< std::setw(2) << std::setfill('0')  << localTime->tm_sec << "] [" << source << "] " << "[INFO] " << buffer;
            break;
        case LogLevel::Warning:
            output << "[" << std::setw(2) << std::setfill('0') << localTime->tm_hour << ":"<< std::setw(2) << std::setfill('0')  << localTime->tm_min << ":"<< std::setw(2) << std::setfill('0')  << localTime->tm_sec << "] [" << source << "] " << "[WARNING] " << buffer;
            break;
        case LogLevel::Error:
            output << "[" << std::setw(2) << std::setfill('0') << localTime->tm_hour << ":"<< std::setw(2) << std::setfill('0')  << localTime->tm_min << ":"<< std::setw(2) << std::setfill('0')  << localTime->tm_sec << "] [" << source << "] " << "[ERROR] " << buffer;
            break;
    }

    #ifdef _WIN32
    switch (level)
    {
        case LogLevel::Info:
            std::cout << output.str() << std::endl;
            break;
        case LogLevel::Warning:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
            std::cout << output.str() << std::endl;
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
        case LogLevel::Error:
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
            std::cout << output.str() << std::endl;
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            break;
    }
    #elif __linux__
    switch (level)
    {
        case LogLevel::Info:
            std::cout << output.str() << std::endl;
            break;
        case LogLevel::Warning:
            std::cout << "\033[33m" << output.str() << "\033[0m" << std::endl;
            break;
        case LogLevel::Error:
            std::cout << "\033[31m" << output.str() << "\033[0m" << std::endl;
            break;
    }
    #else
    std::cout << output.str() << std::endl;
    #endif
}