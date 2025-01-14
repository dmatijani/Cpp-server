#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <netinet/in.h>
#include <thread>
#include <vector>

class Server {
public:
    Server(const std::string& ip_addr, int port);
    ~Server();

    void run();

private:
    std::string ip_address;
    int port;
    int server_socket;
    struct sockaddr_in server_addr;
    std::vector<std::thread> threads;

    void startServer();
    void stopServer();
    void handleClient(int client_socket);
    std::string processRequest(const std::string& request);
    void joinThreads();
};

#endif
