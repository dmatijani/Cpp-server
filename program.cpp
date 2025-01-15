#include "server.h"
#include <csignal>
#include <iostream>

Server *server;

void end (int sig) {
    delete server;
    exit(sig);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Nije unesen argument za port!" << std::endl;
        exit(1);
    }

    int port;
    try {
        port = std::stoi(argv[1]);
    } catch (...) {
        std::cout << "Neispravan port!" << std::endl;
        exit(1);
    }
    std::cout << port << std::endl;

    signal(SIGINT, end);

    server = new Server("127.0.0.1", port);
    server->get("/", "./client/index.html");
    server->get("/autor", "./client/autor.html");
    server->run();

    end(0);

    return 0;
}
