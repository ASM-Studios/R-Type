#include "EntitySchematic.hpp"
#include "GameLogicMode.hpp"
#include "RegistryManager.hpp"
#include "Tags.hpp"
#include "WindowManager.hpp"
#include "query/Header.hpp"
#include "query/NonTypedQuery.hpp"
#include "query/Payloads.hpp"
#include "query/RawRequest.hpp"
#include "query/TypedQuery.hpp"
#include "socket/NRegistry.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#ifdef RTYPE
    constexpr GameLogicMode GAMELOGICMODE(GameLogicMode::CLIENT);
#elif defined(RUNNER)
    constexpr GameLogicMode GAMELOGICMODE(GameLogicMode::RUNNERG);
#endif

int main() {
    ecs::RegistryManager::getInstance().createRegistry();
    Singleton<network::Registry>::wrap();
    Singleton<boost::uuids::uuid>::wrap(boost::uuids::random_generator()());
    try {
        GUI::WindowManager windowManager;
        windowManager.run();
    } catch (const GUI::GuiException& e) {
        Logger::log(LogLevel::ERR, e.what());
        return 1;
    } catch (const libconfig::ParseException &pex) {
        Logger::log(LogLevel::ERR, "Parse error at " + std::string(pex.getFile()) + ":" + std::to_string(pex.getLine()) + " - " + pex.getError());
        return 1;
    }
    return 0;
}
