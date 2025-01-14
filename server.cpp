#include "server.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>

Server::Server(const std::string& ip_addr, int port)
    : ip_address(ip_addr), port(port), server_socket(-1) {
    startServer();
}

Server::~Server() {
    stopServer();
}

void Server::startServer() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        throw std::runtime_error("ERROR: Cannot create socket");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_address.c_str());
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("ERROR: Cannot bind socket");
    }

    if (listen(server_socket, 5) < 0) {
        throw std::runtime_error("ERROR: Cannot listen on socket");
    }

    std::cout << "Server started on " << ip_address << ":" << port << std::endl;
}

void Server::stopServer() {
    if (server_socket >= 0) {
        close(server_socket);
    }
    std::cout << "Server stopped." << std::endl;
}

void Server::run() {
    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            std::cerr << "ERROR: Failed to accept client connection." << std::endl;
            continue;
        }

        std::cout << "Client connected. Processing request..." << std::endl;
        handleClient(client_socket);
    }
}

void Server::handleClient(int client_socket) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        std::cerr << "ERROR: Failed to receive data from client." << std::endl;
        close(client_socket);
        return;
    }

    std::string request(buffer);
    std::string response = processRequest(request);

    send(client_socket, response.c_str(), response.size(), 0);
    close(client_socket);
    std::cout << "Response sent to client and connection closed." << std::endl;
}

std::string Server::processRequest(const std::string& request) {
    if (request.find("GET") != std::string::npos) {
        std::string currentTime = getCurrentTime();
        return "HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n\r\n"
               "<!DOCTYPE html><html><body>" + currentTime + "</body></html>";
    }

    return "HTTP/1.1 400 Bad Request\r\n"
           "Content-Type: text/plain\r\n\r\n"
           "Invalid request.";
}

std::string Server::getCurrentTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M:%S", ltm);
    return std::string(buffer);
}
