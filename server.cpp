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

void Server::get(std::string path, std::string file) {
    use_files[path] = file;
}

void Server::handleClient(int client_socket) {
    std::cout << "Broj dretvi: " << threads.size() << std::endl;
    // TODO: probaj ispisati sve dretve i vidi kako stoje
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
    log("Klijent je dobio odgovor");
}

std::string Server::processRequest(const std::string& request) {
    std::cout << request << std::endl;
    if (request.find("GET") != std::string::npos) {
        std::string data = processGetRequest(request);
        return "HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n\r\n" + data;
    }

    return "HTTP/1.1 400 Bad Request\r\n"
           "Content-Type: text/plain\r\n\r\n"
           "Invalid request.";
}

std::string Server::processGetRequest(const std::string& request) {
    std::string notFound = "<!DOCTYPE html><html lang='en'><p>Nije pronadena stranica koju trazite.</p></html>";

    size_t start = request.find(" ");
    if (start == std::string::npos) {
        return notFound;
    }

    size_t end = request.find(" ", start + 1);
    if (end == std::string::npos) {
        return notFound;
    }

    std::string path = request.substr(start + 1, end - start - 1);
    try {
        std::string file_path = use_files.at(path);
        std::fstream file(file_path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    } catch (...) {
        return notFound;
    }

    return notFound;
}

void Server::joinThreads() {
    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    threads.clear();
}