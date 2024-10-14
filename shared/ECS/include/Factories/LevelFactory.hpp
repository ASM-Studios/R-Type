#pragma once

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <libconfig.h++>
#include <string>
#include <vector>

#include "Logger.hpp"
#include "Registry.hpp"
#include "RegistryManager.hpp"
#include "EntitySchematic.hpp"

namespace ecs::factory {
    constexpr auto SCENARIO_PATH = "shared/Scenarios/scenarios.cfg";
    constexpr int MIN_ALLOWED_ID = 10;

    struct Component {
            std::string type;
            int x;
            int y;
            int spriteID;
            int stateID;
            std::string model;
    };

    struct FactoryEntity {
            std::string name;
            std::size_t id;
            float delay_time;
            std::vector<Component> components;
    };

    inline std::string getScenarioPath(const std::size_t scenarioID) {
        libconfig::Config cfg;
        try {
            cfg.readFile(SCENARIO_PATH);
        } catch (libconfig::FileIOException& e) {
            Logger::log(LogLevel::ERR, "I/O error while reading file: " + std::string(SCENARIO_PATH));
            return "";
        } catch (libconfig::ParseException& e) {
            Logger::log(LogLevel::ERR, "Parse error at " + std::to_string(e.getLine()) + ": " + e.getError());
            return "";
        }

        const libconfig::Setting& root = cfg.getRoot();
        const libconfig::Setting& scenarios = root["scenarios"];

        for (int i = 0; i < scenarios.getLength(); ++i) {
            const libconfig::Setting& scenario = scenarios[i];
            int id;
            scenario.lookupValue("id", id);
            if (id == scenarioID) {
                std::string path;
                scenario.lookupValue("path", path);
                return path;
            }
        }

        Logger::log(LogLevel::ERR, "Scenario ID not found: " + std::to_string(scenarioID));
        return "";
    }

    class LevelFactory {
        public:
            explicit LevelFactory() = default;
            static LevelFactory& getInstance();
            static void load(const std::pair<std::size_t, std::size_t>& screenSize, const std::string& filename);
            static Component parsePosition(const libconfig::Setting& componentSetting, const std::string& type);
            static Component parseSprite(const libconfig::Setting& componentSetting, const std::string& type);
            static Component parseModel(const libconfig::Setting& componentSetting, const std::string& type);
            static void createRegistryEntity(FactoryEntity& entity);
            static void updateEntities(float elapsedTime);
        private:
            static std::vector<FactoryEntity> pendingEntities;
            static std::unique_ptr<LevelFactory> _instance;
            static std::mutex _mutex;
            static std::pair<std::size_t, std::size_t> _screenSize;
    };
}
