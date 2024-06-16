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
    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
    {
        Log::write("Audio", LogLevel::error, "Error initializing audio system: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (Mix_Init(MIX_INIT_OGG) == 0)
    {
        Log::write("Audio", LogLevel::error, "Error initializing SDL2_mixer: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) == -1)
    {
        Log::write("Audio", LogLevel::error, "Error openening audio device: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    Log::write("Audio", LogLevel::info, "Initialized audio engine");
}

bool Audio::loadMusic(const std::string& musicName)
{
    if (musicMap.contains(musicName))
        return true;

    const std::string fileName = "./assets/Music/" + musicName + ".ogg";

    if (Mix_Music* music = Mix_LoadMUS(fileName.c_str()); music == nullptr)
    {
        Log::write("Audio", LogLevel::error, "Can't load music: %s / %s", musicName.c_str(), SDL_GetError());
        return false;
    }
    else
    {
        musicMap.insert({musicName, music});
    }

    Log::write("Audio", LogLevel::info, "Loaded %s music", musicName.c_str());
    return true;
}

void Audio::playMusic(const std::string& musicName, const int loops)
{
    if (!musicMap.contains(musicName) && !loadMusic(musicName))
    {
        return;
    }

    Mix_PlayMusic(musicMap.at(musicName), loops);
}

void Audio::stopMusic()
{
    Mix_HaltMusic();
}

bool Audio::loadSound(const std::string& soundName)
{
    if (soundMap.contains(soundName))
        return true;

    const std::string fileName = "./assets/SFX/" + soundName + ".ogg";
    if (Mix_Chunk* sound = Mix_LoadWAV(fileName.c_str()); sound == nullptr)
    {
        Log::write("Audio", LogLevel::error, "Can't load sound: %s / %s", soundName.c_str(), SDL_GetError());
        return false;
    }
    else
    {
        soundMap.insert({soundName, sound});
    }

    Log::write("Audio", LogLevel::info, "Loaded %s sound", soundName.c_str());
    return true;
}

int Audio::playSound(const std::string& soundName)
{
    if (!soundMap.contains(soundName) && !loadSound(soundName))
    {
        return -1;
    }

    const int channel = Mix_PlayChannel(-1, soundMap.at(soundName), 0);
    if (channel == -1)
    {
        Log::write("Audio", LogLevel::warning, "Can't play sound: %s", SDL_GetError());
    }
    return channel;
}

void Audio::stopSound(const int channel)
{
    Mix_HaltChannel(channel);
}

void Audio::unloadAllMusic()
{
    for (const auto& [musicName, music] : musicMap)
    {
        Log::write("Audio", LogLevel::info, "Unloaded %s music", musicName.c_str());
        Mix_FreeMusic(music);
    }
    musicMap.clear();
}

void Audio::unloadAllSounds()
{
    for (const auto& [soundName, sound] : soundMap)
    {
        Log::write("Audio", LogLevel::info, "Unloaded %s sound", soundName.c_str());
        Mix_FreeChunk(sound);
    }
    soundMap.clear();
}

void Audio::cleanUp()
{
    unloadAllMusic();
    unloadAllSounds();
    Mix_CloseAudio();
}