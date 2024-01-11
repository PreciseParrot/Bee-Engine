#pragma once

#include <string>

namespace Audio
{
    void loadMusic(std::string musicName);
    void playMusic(std::string musicName, int loops);
    void stopMusic();
    void loadSound(std::string soundName);
    int playSound(std::string soundName);
    void stopSound(int channel);
};