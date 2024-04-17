#include "Audio.hpp"

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
        Log::write("Audio", LogLevel::Error, "Error initializing SDL2_mixer: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) == -1)
    {
        Log::write("Audio", LogLevel::Error, "Error openening audio device: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    Log::write("Audio", LogLevel::Info, "Initialized audio engine");
}

void Audio::loadMusic(const std::string& musicName)
{
    if (musicMap.find(musicName) != musicMap.end())
        return;

    std::string fileName = "./assets/Music/" + musicName + ".ogg";
    Mix_Music* music = Mix_LoadMUS(fileName.c_str());
    if (music == nullptr)
    {
        Log::write("Audio", LogLevel::Warning, "Can't load music: %s", SDL_GetError());
    }
    else
    {
        musicMap.insert({musicName, music});
        Log::write("Audio", LogLevel::Info, "Loaded %s music", musicName.c_str());
    }
}

void Audio::playMusic(const std::string& musicName, int loops)
{
    loadMusic(musicName);
    Mix_PlayMusic(musicMap.at(musicName), loops);
}

void Audio::stopMusic()
{
    Mix_HaltMusic();
}

void Audio::loadSound(const std::string& soundName)
{
    if (soundMap.find(soundName) != soundMap.end())
        return;

    std::string fileName = "./assets/SFX/" + soundName + ".ogg";
    Mix_Chunk* sound = Mix_LoadWAV(fileName.c_str());
    if (sound == nullptr)
    {
        Log::write("Audio", LogLevel::Error, "Can't load sound: %s", SDL_GetError());
    }
    else
    {
        soundMap.insert({soundName, sound});
        Log::write("Audio", LogLevel::Info, "Loaded %s sound", soundName.c_str());
    }
}

int Audio::playSound(const std::string& soundName)
{
    loadSound(soundName);

    int channel = Mix_PlayChannel(-1, soundMap.at(soundName), 0);
    if (channel == -1)
    {
        Log::write("Audio", LogLevel::Warning, "Can't play sound: %s", SDL_GetError());
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
        Log::write("Audio", LogLevel::Info, "Unloaded %s music", musicName.c_str());
        Mix_FreeMusic(music);
    }
    musicMap.clear();
}

void Audio::unloadAllSounds()
{
    for (const auto& [soundName, sound] : soundMap)
    {
        Log::write("Audio", LogLevel::Info, "Unloaded %s sound", soundName.c_str());
        Mix_FreeChunk(sound);
    }
    soundMap.clear();
}

void Audio::cleanUp()
{
    Audio::unloadAllMusic();
    Audio::unloadAllSounds();
    Mix_CloseAudio();
}