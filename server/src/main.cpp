#include "Core.hpp"

int main(int ac, char **av) {
    auto args = std::span<char *>(av, ac);
    Core core;

    core.init(args);
    return core.run();
}
