#include "server.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <thread>
#include <vector>
#include <fstream>

void log(const std::string &message) {
    std::cout << message << std::endl;
}

void logError(const std::string &errorMessage) {
    log("ERROR: " + errorMessage);
}

Server::Server(const std::string& ip_addr, int port)
    : ip_address(ip_addr), port(port), server_socket(-1) {
    startServer();
}

Server::~Server() {
    stopServer();
    joinThreads();
}

void Server::startServer() {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        logError("Ne mogu kreirati socket");
        exit(1);
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        logError("Ne mogu postaviti opciju SO_REUSEADDR");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_address.c_str());
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        logError("Ne mogu povezati socket");
        exit(1);
    }

    if (listen(server_socket, 5) < 0) {
        logError("Ne mogu slušati na socketu");
        exit(1);
    }

    log("Server pokrenut na " + ip_address + ":" + std::to_string(port));
}

void Server::stopServer() {
    if (server_socket >= 0) {
        close(server_socket);
        log("Socket zatvoren");
    }
    log("Server uspješno ugašen");
}

void Server::run() {
    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            logError("Klijent se ne može povezati");
            continue;
        }

        log("Klijent se povezao");
        threads.emplace_back(&Server::handleClient, this, client_socket);
    }
}

void Server::get(std::string path, std::string(*callback)(Request*)) {
    handlers["GET"].insert(std::make_pair(path, callback));
}

void Server::post(std::string path, std::string(*callback)(Request*)) {
    handlers["POST"].insert(std::make_pair(path, callback));
}

void Server::handleClient(int client_socket) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        logError("Nemogućnost prihvaćanja podataka od klijenta");
        close(client_socket);
        return;
    }

    std::string request(buffer);
    std::string response = processRequest(request);

    send(client_socket, response.c_str(), response.size(), 0);
    close(client_socket);
}

std::string Server::processRequest(const std::string& request_text) {
    std::string bad_request = "HTTP/1.1 400 Bad Request\r\n"
                              "Content-Type: text/plain\r\n\r\n"
                              "Invalid request.";
    std::string not_found = "<!DOCTYPE html><html lang='en'><p>Nije pronadena stranica koju trazite.</p></html>";
    Request* request = new Request(request_text);
    auto handler = handlers.at(request->method);

    try {
        std::string(*callback)(Request*) = handler.at(request->path);

        std::string file_text = callback(request);

        delete request;
        return "HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n\r\n" + file_text;
    } catch (...) {
        delete request;
        return "HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n\r\n" + not_found;
    }

    delete request;
    return bad_request;
}

void Server::joinThreads() {
    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
            log("Dretva je joinana");
        } else {
            log("Dretva se ne može joinati");
        }
    }
    threads.clear();
}