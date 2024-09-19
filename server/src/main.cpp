#include "Logging.hpp"
#include "QueryHandler.hpp"
#include "socket/Server.hpp"
#include <csignal>
#include <fcntl.h>
#include <span>
#include <unistd.h>

int main(int ac, char **av) {
    auto args = std::span<char *>(av, ac);
    network::socket::udp::Server server(8080);

    while (true) {
        if (server.availableRequest()) {
            auto query = server.recv<Query>();
            network::QueryHandler::getInstance()->addQuery(query);
        }
        network::QueryHandler::getInstance()->executeQueries();
        network::QueryHandler::getInstance()->checkWorkers();
    }
    return 0;
}
