#include "server.h"
#include <csignal>
#include <iostream>

std::unique_ptr<Server> server;

void end (int sig) {
    server.reset();
    exit(sig);
}

int main() {
    server = std::make_unique<Server>("0.0.0.0", 12345);
    server->run();

    sigset(SIGINT, end);

    return 0;
}
