#include "Audio.hpp"

#include <iostream>
#include <unordered_map>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "Log.hpp"

static std::unordered_map<std::string, Mix_Music*> musicMap;
static std::unordered_map<std::string, Mix_Chunk*> soundMap;   

void Audio::init()
{
    if (Mix_Init(MIX_INIT_OPUS) == 0)
    {
        Log::write("Audio", LogLevel::Error, "Error initializing SDL2_mixer: ", SDL_GetError());
        throw std::exception();
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) == -1)
    {
        Log::write("Audio", LogLevel::Error, "Error openening audio device: ", SDL_GetError());
        throw std::exception();
    }

    Log::write("Audio", LogLevel::Info, "Initialized audio engine");
}

void Audio::loadMusic(std::string musicName)
{
    std::string fileName = "./assets/Music/" + musicName + ".ogg";
    Mix_Music* music = Mix_LoadMUS(fileName.c_str());
    if (music == nullptr)
    {
        Log::write("Audio", LogLevel::Error, "Error loading music: ", SDL_GetError());
        throw std::exception();
    }
    musicMap.insert({musicName, music});
    Log::write("Audio", LogLevel::Info, "Loaded " + musicName + " music");
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
        Log::write("Audio", LogLevel::Error, "Error loading sound: ", SDL_GetError());
        throw std::exception();
    }
    soundMap.insert({soundName, sound});
    Log::write("Audio", LogLevel::Info, "Loaded " + soundName + " sound");
}

int Audio::playSound(std::string soundName)
{
    int channel = Mix_PlayChannel(-1, soundMap.at(soundName), 0);
    if (channel == -1)
    {
        Log::write("Audio", LogLevel::Warning, "Can't play sound: ", SDL_GetError());
    }
    return channel;
}

void Audio::stopSound(int channel)
{
    Mix_HaltChannel(channel);
}

void Audio::unloadAllMusic()
{
    for (const auto& [musicName, music] : musicMap)
    {
        Log::write("Audio", LogLevel::Info, "Unloaded " + musicName + " music");
        Mix_FreeMusic(music);
    }
}

void Audio::unloadAllSounds()
{
    for (const auto& [soundName, sound] : soundMap)
    {
        Log::write("Audio", LogLevel::Info, "Unloaded " + soundName + " sound");
        Mix_FreeChunk(sound);
    }
}

void Audio::cleanUp()
{
    Audio::unloadAllMusic();
    Audio::unloadAllSounds();
    Mix_CloseAudio();
}