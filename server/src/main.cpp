#include "Logging.hpp"
#include "QueryHandler.hpp"
#include "socket/Server.hpp"
#include <algorithm>
#include <csignal>
#include <fcntl.h>
#include <iostream>
#include <span>
#include <unistd.h>

bool running = true;

void sigHandler(int signum) {
    running = false;
}

int main(int ac, char **av) {
    signal(SIGINT, &sigHandler);
    auto args = std::span<char *>(av, ac);
    network::socket::udp::Server server(8080);

    while (running) {
        if (server.availableRequest()) {
            auto query = server.recv<Query>();
            network::QueryHandler::getInstance()->addQuery(query);
        }
        network::QueryHandler::getInstance()->executeQueries();
        network::QueryHandler::getInstance()->checkWorkers();
    }
    return 0;
}
