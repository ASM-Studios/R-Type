#include "MusicManager.hpp"

GUI::MusicManager::MusicManager() : _volume(50.0f), _isMuted(false) {
    Config &config = Config::getInstance("client/config.json");
    loadMusic(config.get("musics_config_path").value_or(DEFAULT_MUSIC_CONFIG));
    _musicsPath = config.get("musics_path").value_or(DEFAULT_MUSIC_PATH);
}

void GUI::MusicManager::loadMusic(const std::string& configPath) {
    libconfig::Config cfg;
    try {
        cfg.readFile(configPath.c_str());
    } catch (const libconfig::FileIOException &fioex) {
        throw std::runtime_error("I/O error while reading file.");
    } catch (const libconfig::ParseException &pex) {
        throw std::runtime_error("Parse error at " + std::string(pex.getFile()) + ":" + std::to_string(pex.getLine()) + " - " + pex.getError());
    }

    const libconfig::Setting& root = cfg.getRoot();
    if (root.exists("musics")) {
        const libconfig::Setting& musicSettings = root["musics"];
        for (int i = 0; i < musicSettings.getLength(); ++i) {
            const libconfig::Setting& music = musicSettings[i];
            std::string name, path;
            if (music.lookupValue("name", name) && music.lookupValue("path", path)) {
                auto musicPtr = std::make_shared<sf::Music>();
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

void GUI::MusicManager::setVolume(float volume) {
    _volume = std::clamp(volume, 0.0f, 100.0f);
    std::cout << "Setting volume to: " << _volume << std::endl;
    _updateVolume();
}

float GUI::MusicManager::getVolume() const {
    std::cout << "Current volume: " << _volume << std::endl;
    return _volume;
}

void GUI::MusicManager::toggleMute() {
    _isMuted = !_isMuted;
    _updateVolume();
}

void GUI::MusicManager::setMute(bool mute) {
    _isMuted = mute;
    _updateVolume();
}

void GUI::MusicManager::setMusic(const std::string& musicName, bool loop) {
    if (_musicMap.find(musicName) != _musicMap.end()) {
        if (_currentMusic) {
            _currentMusic->stop();
        }
        _currentMusic = _musicMap[musicName];
        _currentMusic->setLoop(loop);
        _currentMusic->setVolume(_isMuted ? 0.0f : _volume);
        _currentMusic->play();
    } else {
        throw std::runtime_error("Music not found: " + musicName);
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
