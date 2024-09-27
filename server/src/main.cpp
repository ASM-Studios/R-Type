#include "Core.hpp"
#include "Logging.hpp"

static void setLogLevel() {
#ifndef NDEBUG
    Logging::setLogLevel(Logging::Level::ALL);
#else
    Logging::setLogLevel(Logging::Level::WARN);
#endif
}

int main(int ac, char **av) {
    setLogLevel();
    auto args = std::span<char *>(av, ac);
    Core core;

    core.init(args);
    return core.run();
}
