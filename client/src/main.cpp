#include "WindowManager.hpp"

int main() {
    try {
        GUI::WindowManager const windowManager;
        windowManager.run();
    } catch (const GUI::GuiException& e) {
        Logger::log(LogLevel::ERROR, e.what());
        return 1;
    }
    return 0;
}
