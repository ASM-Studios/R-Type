#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <memory>
#include <algorithm>
#include <libconfig.h++>

#include "Logger.hpp"

namespace GUI {
    constexpr auto DEFAULT_MUSIC_CONFIG = "client/assets/musics_config.cfg";
    constexpr auto DEFAULT_MUSIC_PATH = "client/assets/musics/";

    class MusicManager {
        private:
            void loadMusic(const std::string& configPath);

            std::unordered_map<std::string, std::shared_ptr<sf::Music>> _musicMap;
            std::shared_ptr<sf::Music> _currentMusic;
            std::string _musicsPath;
            float _volume;
            float _volumeBeforeMute;
            bool _isMuted;

            void _updateMute();
            void _updateVolume();

        public:
            explicit MusicManager();
            ~MusicManager() = default;

            void setVolume(float volume);
            float getVolume() const;
            bool isMuted() const;
            void toggleMute();
            void setMute(bool mute);
            void setMusic(const std::string& musicName, bool loop = true);
    };
}
