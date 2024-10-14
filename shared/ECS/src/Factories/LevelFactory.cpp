#include "Factories/LevelFactory.hpp"

namespace ecs::factory {
    void LevelFactory::load(const std::pair<std::size_t, std::size_t>& screenSize, const std::string& filename) {
        libconfig::Config cfg;
        try {
            cfg.readFile(filename.c_str());
        } catch (libconfig::FileIOException& e) {
            Logger::log(LogLevel::ERR, "I/O error while reading file: " + filename);
            return;
        } catch (libconfig::ParseException& e) {
            Logger::log(LogLevel::ERR, "Parse error at " + std::to_string(e.getLine()) + ": " + e.getError());
            return;
        }

        std::vector<FactoryEntity> entities;
        const libconfig::Setting& root = cfg.getRoot();
        const libconfig::Setting& entitiesSetting = root["entities"];

        for (int i = 0; i < entitiesSetting.getLength(); ++i) {
            const libconfig::Setting& entitySetting = entitiesSetting[i];
            FactoryEntity entity;
            entitySetting.lookupValue("name", entity.name);
            int id;
            entitySetting.lookupValue("id", id);
            entity.id = static_cast<std::size_t>(id);
            if (id < MIN_ALLOWED_ID) {
                Logger::log(LogLevel::WARNING, "Entity ID is less than " + std::to_string(MIN_ALLOWED_ID) + ", skipping.");
                continue;
            }

            const libconfig::Setting& componentsSetting = entitySetting["components"];
            for (int j = 0; j < componentsSetting.getLength(); ++j) {
                const libconfig::Setting& componentSetting = componentsSetting[j];
                Component component;
                componentSetting.lookupValue("type", component.type);

                if (component.type == "Position") {
                    if (componentSetting.exists("x") && componentSetting.exists("y")) {
                        component = parsePosition(componentSetting, component.type);
                    } else {
                        Logger::log(LogLevel::WARNING, "Position component missing 'x' or 'y' value, skipping.");
                        continue;
                    }
                } else if (component.type == "Sprite") {
                    if (componentSetting.exists("spriteID") && componentSetting.exists("stateID")) {
                        component = parseSprite(componentSetting, component.type);
                    } else {
                        Logger::log(LogLevel::WARNING, "Sprite component missing 'spriteID' or 'stateID' value, skipping.");
                        continue;
                    }
                } else if (component.type == "AI") {
                    if (componentSetting.exists("model")) {
                        component = parseModel(componentSetting, component.type);
                    } else {
                        Logger::log(LogLevel::WARNING, "AI component missing 'model' value, skipping.");
                        continue;
                    }
                } else {
                    Logger::log(LogLevel::ERR, "Unknown component type: " + component.type);
                }
                entity.components.push_back(component);
            }
            entities.push_back(entity);
        }
        try {
            createRegistryEntity(entities, screenSize);
        } catch (const std::exception& e) {
            Logger::log(LogLevel::ERR, e.what());
        }
    }

    Component LevelFactory::parsePosition(const libconfig::Setting& componentSetting, const std::string& type) {
        Component component;
        component.type = type;
        componentSetting.lookupValue("x", component.x);
        componentSetting.lookupValue("y", component.y);
        return component;
    }

    Component LevelFactory::parseSprite(const libconfig::Setting& componentSetting, const std::string& type) {
        Component component;
        component.type = type;
        componentSetting.lookupValue("spriteID", component.spriteID);
        componentSetting.lookupValue("stateID", component.stateID);
        return component;
    }

    Component LevelFactory::parseModel(const libconfig::Setting& componentSetting, const std::string& type) {
        Component component;
        component.type = type;
        componentSetting.lookupValue("model", component.model);
        return component;
    }

    void LevelFactory::createRegistryEntity(const std::vector<FactoryEntity>& entities, const std::pair<std::size_t, std::size_t>& _screenSize) {
        for (const auto& [name, id, components]: entities) {
            int16_t x = 0;
            int16_t y = 0;
            int spriteID = 0;
            int stateID = 0;
            std::string model;

            for (const auto& [type, cx, cy, cspriteID, cstateID, cmodel]: components) {
                if (type == "Position") {
                    x = static_cast<int16_t>(cx);
                    y = static_cast<int16_t>(cy);
                } else if (type == "Sprite") {
                    spriteID = cspriteID;
                    stateID = cstateID;
                } else if (type == "AI") {
                    model = cmodel;
                }
            }
            EntitySchematic::createEnemy(id, x, y, spriteID, stateID, model, _screenSize);
        }
    }
}
