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

constexpr GameLogicMode GAMELOGICMODE(GameLogicMode::CLIENT);

int main() {
    try {
        GUI::WindowManager windowManager;
        windowManager.run();
    } catch (const GUI::GuiException& e) {
        Logger::log(LogLevel::ERR, e.what());
        return 1;
    }
    return 0;
}
