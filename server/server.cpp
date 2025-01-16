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
        std::cerr << "Ne mogu kreirati socket" << std::endl;
        exit(1);
    }

    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "Ne mogu postaviti opciju SO_REUSEADDR" << std::endl;
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip_address.c_str());
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Ne mogu povezati socket" << std::endl;
        exit(1);
    }

    if (listen(server_socket, 5) < 0) {
        std::cerr << "Ne mogu slušati na socketu" << std::endl;
        exit(1);
    }

    std::cout << "Server pokrenut na " << ip_address << ":" << port << std::endl << std::endl;
}

void Server::stopServer() {
    if (server_socket >= 0) {
        close(server_socket);
        std::cout << "Socket zatvoren" << std::endl;
    }
    std::cout << "Server uspješno ugašen" << std::endl;
}

void Server::run() {
    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        if (client_socket < 0) {
            std::cerr << "Klijent se ne može povezati" << std::endl;
            continue;
        }

        std::string client_ip = inet_ntoa(client_addr.sin_addr);

        threads.emplace_back(&Server::handleClient, this, client_socket, client_ip);
    }
}

void Server::get(std::string path, void(*callback)(Request*, Response*)) {
    handlers["GET"].insert(std::make_pair(path, callback));
}

void Server::post(std::string path, void(*callback)(Request*, Response*)) {
    handlers["POST"].insert(std::make_pair(path, callback));
}

void Server::handleClient(int client_socket, std::string client_ip) {
    char buffer[2048];
    memset(buffer, 0, sizeof(buffer));

    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        if (bytes_received == -1) {
            std::cerr << "Nemogućnost prihvaćanja podataka od klijenta" << std::endl;
        }
        close(client_socket);
        return;
    }

    std::string request(buffer);
    std::string response = processRequest(request, client_ip);

    send(client_socket, response.c_str(), response.size(), 0);
    close(client_socket);
}

std::string Server::processRequest(const std::string& request_text, std::string client_ip) {
    Request* request = new Request(request_text, client_ip);
    if (!request->is_valid()) {
        request->print();
        delete request;
        return Response::bad_request_text();
    }
    Response* response = new Response();
    auto handler_iter = handlers.find(request->method);
    if (handler_iter == handlers.end()) {
        request->print();
        return Response::bad_request_text();
    }
    auto handler = handler_iter->second;

    try {
        void(*callback)(Request*, Response*) = handler.at(request->path);
        callback(request, response);
        request->print();

        std::string response_text = response->text();
        delete request;
        delete response;
        return response_text;
    } catch (...) {
        request->print();
        delete request;
        delete response;
        return Response::not_found_text();
    }

    delete request;
    return Response::bad_request_text();
}

void Server::joinThreads() {
    std::cout << "Joinam dretve" << std::endl;
    for (std::thread& t : threads) {
        if (t.joinable()) {
            t.join();
        } else {
            std::cerr << "Dretva " << t.get_id() << " se ne može joinati" << std::endl;
        }
    }
    threads.clear();
}