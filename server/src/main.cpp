#include "Core.hpp"
#include "Entity.hpp"
#include "EntitySchematic.hpp"
#include "GameLogic.hpp"
#include "GameLogicManager.hpp"
#include "GameLogicMode.hpp"
#include "Logger.hpp"
#include "RegistryManager.hpp"
#include "Tags.hpp"
#include "query/Header.hpp"
#include "query/Payloads.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include "socket/Client.hpp"
#include "socket/NRegistry.hpp"
#include "socket/NetworkManager.hpp"
#include <boost/asio/buffer.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <csignal>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <memory>

constexpr GameLogicMode GAMELOGICMODE(GameLogicMode::SERVER);

void hexDisplay(const char *ptr, std::size_t n) {
    for (int i = 0; i < n; i++) {
        if ((i > 0) && (i % 20 == 0)) {
            std::cout << std::endl;
        }
        std::cout << std::setbase(16) << std::setw(2) << std::setfill('0') << static_cast<int>(ptr[i] & 0xff) << " ";
    }
    std::cout << std::endl
              << std::endl;
}

static void initSingleton() {
    Singleton<network::Registry>::wrap();
    Singleton<boost::uuids::uuid>::wrap();

    Singleton<boost::uuids::uuid>::getInstance().lock();
    auto& uuid = Singleton<boost::uuids::uuid>::getInstance().get();
    std::fill(uuid.begin(), uuid.end(), 0);
    Singleton<boost::uuids::uuid>::getInstance().unlock();
}

int main(int ac, char **av) {
    initSingleton();
    GameLogicManager::getInstance();
    auto args = std::span<char *>(av, ac);
    Core core;

    core.init(args);
    return core.run();
}
