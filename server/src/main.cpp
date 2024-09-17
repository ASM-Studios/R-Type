#include "socket/Server.hpp"
#include <fcntl.h>
#include <iostream>
#include <span>
#include <unistd.h>

int main(int ac, char **av) {
    auto args = std::span<char *>(av, ac);
    udp_socket::Server server(8080);

    while (true) {
        if (server.availableRequest()) {
            auto query = server.recv<char>();
            std::cout << "Receiving data" << std::endl;
        }
    }
    return 0;
}
