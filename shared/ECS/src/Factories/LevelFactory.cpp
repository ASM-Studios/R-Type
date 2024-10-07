#include "Factories/LevelFactory.hpp"

namespace ecs::factory {
    LevelFactory::LevelFactory(Registry& registry, const std::pair<std::size_t, std::size_t>& screenSize, const std::string& filename) {
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
            createRegistryEntity(entities, registry, screenSize);
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

    void LevelFactory::createRegistryEntity(const std::vector<FactoryEntity>& entities, Registry& _registry, const std::pair<std::size_t, std::size_t>& _screenSize) {
        for (const auto&[name, components] : entities) {
            auto newEntity = _registry.createEntity<>();
            for (const auto&[type, x, y, spriteID, stateID, model] : components) {
                if (type == "Position") {
                    _registry.setComponent<ecs::component::Position>(newEntity, {static_cast<int16_t>(x), static_cast<int16_t>(y), _screenSize.first, _screenSize.second});
                } else if (type == "Sprite") {
                    _registry.setComponent<ecs::component::Sprite>(newEntity, {spriteID, stateID});
                } else if (type == "AI") {
                    _registry.setComponent<ecs::component::AI>(newEntity, {ecs::component::AI::stringToAIModel(model)});
                }
            }
        }
    }
}
