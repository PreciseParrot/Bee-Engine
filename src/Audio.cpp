#include "Audio.hpp"

#include <iostream>
#include <unordered_map>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

static std::unordered_map<std::string, Mix_Music*> musicMap;
static std::unordered_map<std::string, Mix_Chunk*> soundMap;   

void Audio::init()
{
    if (Mix_Init(MIX_INIT_OPUS) == 0)
    {
        std::cout << "Error initializing SDL2_mixer: " << SDL_GetError() << std::endl;
        throw std::exception();
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) == -1)
    {
        std::cout << "Error openening audio device: " << SDL_GetError() << std::endl;
        throw std::exception();
    }

    std::cout << "Initialized audio engine" << std::endl;
}

void Audio::loadMusic(std::string musicName)
{
    std::string fileName = "./assets/Music/" + musicName + ".ogg";
    Mix_Music* music = Mix_LoadMUS(fileName.c_str());
    if (music == nullptr)
    {
        std::cout << "Error loading music: " << SDL_GetError() << std::endl;
        throw std::exception();
    }
    musicMap.insert({musicName, music});
}

void Audio::playMusic(std::string musicName, int loops)
{
    Mix_PlayMusic(musicMap.at(musicName), loops);
}

void Audio::stopMusic()
{
    Mix_HaltMusic();
}

void Audio::loadSound(std::string soundName)
{
    std::string fileName = "./assets/SFX/" + soundName + ".ogg";
    Mix_Chunk* sound = Mix_LoadWAV(fileName.c_str());
    if (sound == nullptr)
    {
        std::cout << "Error loading sound: " << SDL_GetError() << std::endl;
        throw std::exception();
    }
    soundMap.insert({soundName, sound});
}

int Audio::playSound(std::string soundName)
{
    int channel = Mix_PlayChannel(-1, soundMap.at(soundName), 0);
    if (channel == -1)
    {
        std::cout << "Error playing sound: " << SDL_GetError() << std::endl;
    }
    return channel;
}

void Audio::stopSound(int channel)
{
    Mix_HaltChannel(channel);
}

void Audio::unloadAllMusic()
{
    for (const auto& [key, val] : musicMap)
    {
        std::cout << "Unloaded " << key << " music" << std::endl;
        Mix_FreeMusic(val);
    }
}

void Audio::unloadAllSounds()
{
    for (const auto& [key, val] : soundMap)
    {
        std::cout << "Unloaded " << key << " sound" << std::endl;
        Mix_FreeChunk(val);
    }
}

void Audio::cleanUp()
{
    Audio::unloadAllMusic();
    Audio::unloadAllSounds();
    Mix_CloseAudio();
}