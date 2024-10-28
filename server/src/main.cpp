#include "socket/Client.hpp"
#include "Core.hpp"
#include "Entity.hpp"
#include "EntitySchematic.hpp"
#include "GameLogicMode.hpp"
#include "Logger.hpp"
#include "RegistryManager.hpp"
#include "Tags.hpp"
#include "query/Header.hpp"
#include "query/Payloads.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include "socket/NRegistry.hpp"
#include "socket/NetworkManager.hpp"
#include <boost/asio/buffer.hpp>
#include <boost/uuid/random_generator.hpp>
#include <csignal>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <boost/uuid/uuid_io.hpp>
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
    auto& uuid = Singleton<boost::uuids::uuid>::getInstance().get();
    std::fill(uuid.begin(), uuid.end(), 0);

}

int main(int ac, char **av) {
    initSingleton();
    auto args = std::span<char *>(av, ac);
    Core core;

    core.init(args);
    return core.run();
}
