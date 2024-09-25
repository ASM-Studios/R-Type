#include "Core.hpp"
#include "Logging.hpp"
#include <unistd.h>

static void displayPID() {
    Logging::info(std::format("PID: {}", getpid()));
}

static void setLogLevel() {
#ifndef NDEBUG
    Logging::setLogLevel(Logging::Level::ALL);
#else
    Logging::setLogLevel(Logging::Level::WARN);
#endif
}

int main(int ac, char **av) {
    setLogLevel();
    displayPID();
    auto args = std::span<char *>(av, ac);
    Core core;

    core.init(args);
    return core.run();
}
