#pragma once

#include <libconfig.h++>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <iostream>
#include <string>
#include <vector>

#include "Logger.hpp"
#include "Registry.hpp"

namespace ecs::factory {
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
        std::vector<Component> components;
    };

    class LevelFactory {
        public:
            explicit LevelFactory(Registry& registry, const std::pair<std::size_t, std::size_t>& screenSize, const std::string& filename);

	    static void load(const std::string& filename);
	    static Component parsePosition(const libconfig::Setting& componentSetting, const std::string& type);
            static Component parseSprite(const libconfig::Setting& componentSetting, const std::string& type);
            static Component parseModel(const libconfig::Setting& componentSetting, const std::string& type);

            static void createRegistryEntity(const std::vector<FactoryEntity>& entities, Registry& _registry, const std::pair<std::size_t, std::size_t>& _screenSize);
    };
}
