#include "WindowManager.hpp"
#include "query/Header.hpp"
#include "query/RawRequest.hpp"

const std::map<RequestType, void (*)(network::Client client, RawRequest rawRequest)> requestAction = {
    {NOTHING, [](network::Client client, RawRequest rawRequest) {return;}}
};

int main() {
    const Config& config = Config::getInstance("client/config.json");
    try {
        GUI::WindowManager windowManager(config);
        windowManager.run();
    } catch (const GUI::GuiException& e) {
        Logger::log(LogLevel::ERR, e.what());
        return 1;
    }
    return 0;
}
