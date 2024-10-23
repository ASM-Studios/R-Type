#include "MusicManager.hpp"

#include <GuiException.hpp>

/**
 * \brief Constructs a MusicManager object.
 *
 * Initializes the MusicManager with default volume and mute settings.
 * Loads the music configuration from the specified config file.
 *
 * \throws GuiException if there is an error reading the config file.
 */
GUI::MusicManager::MusicManager() : _volume(0.0f), _isMuted(false) {
    try {
        Config &config = Config::getInstance("client/config.json");
        loadMusic(config.get("musics_config_path").value_or(DEFAULT_MUSIC_CONFIG));
        _musicsPath = config.get("musics_path").value_or(DEFAULT_MUSIC_PATH);
    } catch (std::runtime_error &e) {
        throw GuiException(e.what());
    }
}

/**
 * \brief Loads the musics files from the specified configuration file.
 *
 * \param configPath The path to the music configuration file.
 *
 * \throws GuiException if there is an error reading the config file.
 */
void GUI::MusicManager::loadMusic(const std::string& configPath) {
    libconfig::Config cfg;
    try {
        cfg.readFile(configPath.c_str());
    } catch (const libconfig::FileIOException) {
        throw GuiException("I/O error while reading file.");
    } catch (const libconfig::ParseException &pex) {
        throw GuiException("Parse error at " + std::string(pex.getFile()) + ":" + std::to_string(pex.getLine()) + " - " + pex.getError());
    }

    if (const libconfig::Setting& root = cfg.getRoot(); root.exists("musics")) {
        const libconfig::Setting& musicSettings = root["musics"];
        for (int i = 0; i < musicSettings.getLength(); ++i) {
            const libconfig::Setting& music = musicSettings[i];
            std::string name;
            if (std::string path; music.lookupValue("name", name) && music.lookupValue("path", path)) {
                const auto musicPtr = std::make_shared<sf::Music>();
                std::string fullPath = _musicsPath + path;
                if (!musicPtr->openFromFile(fullPath)) {
                    Logger::log(LogLevel::ERR, "Failed to load music: " + fullPath);
                }
                _musicMap[name] = musicPtr;
                Logger::log(LogLevel::INFO, "Loaded music: " + name + " from " + fullPath);
            }
        }
    }
}

void GUI::MusicManager::setVolume(const float volume) {
    _volume = std::clamp(volume, 0.0f, 100.0f);
    _updateVolume();
}

float GUI::MusicManager::getVolume() const {
    return _volume;
}

void GUI::MusicManager::toggleMute() {
    _isMuted = !_isMuted;
    _updateVolume();
}

void GUI::MusicManager::setMute(const bool mute) {
    _isMuted = mute;
    _updateVolume();
}

/**
 * \brief Sets the music to be played.
 *
 * \param musicName The name of the music to be played.
 * \param loop Whether the music should loop or not.
 *
 * \throws GuiException if the music is not found.
 */
void GUI::MusicManager::setMusic(const std::string& musicName, const bool loop) {
    if (_musicMap.contains(musicName)) {
        if (_currentMusic) {
            _currentMusic->stop();
        }
        _currentMusic = _musicMap[musicName];
        _currentMusic->setLoop(loop);
        _currentMusic->setVolume(_isMuted ? 0.0f : _volume);
        _currentMusic->play();
    } else {
        throw GuiException("Music not found: " + musicName);
    }
}

void GUI::MusicManager::_updateMute() {
    if (_isMuted) {
        _volumeBeforeMute = _volume;
        _volume = 0.0f;
    } else {
        _volume = _volumeBeforeMute;
    }
}

void GUI::MusicManager::_updateVolume() {
    if (_currentMusic) {
        _currentMusic->setVolume(_isMuted ? 0.0f : _volume);
    }
}
