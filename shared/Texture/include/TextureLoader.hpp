#pragma once

#include "Texture.hpp"
#include <cstddef>
#include <libconfig.h++>
#include <memory>
#include <mutex>
#include <unordered_map>

class TextureLoader {
    public:
        enum class Type {
            BACKGROUND,
            BULLET,
            ENEMY,
            SHIP,
            EXPLOSION,
            PLAYER,
            TRAP,
            PLATFORM
        };

    private:
        static std::unique_ptr<TextureLoader> _instance;
        static std::mutex _mutex;

        explicit TextureLoader() = default;

        libconfig::Config _cfg;
        std::unordered_map<int, std::pair<Type, Texture>> _textures;

    public:
        TextureLoader(const TextureLoader& other) = delete;
        ~TextureLoader() = default;
        TextureLoader& operator=(const TextureLoader& other) = delete;

        static TextureLoader& getInstance();

        void loadFile(const std::string& path);
        void loadTexture(const std::string& path, Type type);
        void loadTextures(std::string path, Type type);

        Texture& getTexture(int id);
        std::size_t getNoTexture() const;
        std::pair<int, int> getSizeFromId(int textureId);
};
