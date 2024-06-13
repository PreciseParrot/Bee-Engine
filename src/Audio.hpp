/**
 * @file Audio.hpp
 */

#pragma once

#include <string>

/**
 * @namespace Audio
 * 
 * @brief All the audio related functions.
 * 
 */
namespace Audio
{
    /*Internal functions start here*/

    void init();
    void cleanUp();

    /*Internal functions end here*/

    /**
     * @brief Preload a song into memory.
     * 
     * @param musicName the name of the song
     */
    bool loadMusic(const std::string& musicName);

    /**
     * @brief Play a song.
     * 
     * @param musicName the name of the song.
     * @param loops the number of loops to play the song for
     */
    void playMusic(const std::string& musicName, int loops);

    /**
     * @brief Stop any playing music.
     * 
     */
    void stopMusic();

    /**
     * @brief Preload a sound into memory.
     * 
     * @param soundName the name of the sound
     */
    bool loadSound(const std::string& soundName);

    /**
     * @brief Play a sound.
     * 
     * @param soundName the name of the sound.
     * @return the channel id on which the sound is played.
     */
    int playSound(const std::string& soundName);

    /**
     * @brief Stop a playing sound.
     * 
     * @param channel the channel id of the sound
     */
    void stopSound(int channel);

    /**
     * @brief Unload all loaded songs.
     * 
     */
    void unloadAllMusic();

    /**
     * @brief Unload all loaded sounds.
     * 
     */
    void unloadAllSounds();
};