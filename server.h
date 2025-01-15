#include "request.h"
#include "response.h"
#include <string>
#include <netinet/in.h>
#include <thread>
#include <vector>
#include <map>

class Server {
public:
    Server(const std::string& ip_addr, int port);
    ~Server();

    void run();
    void get(std::string path, std::string(*callback)(Request*, Response*));
    void post(std::string path, std::string(*callback)(Request*, Response*));

private:
    std::string ip_address;
    int port;
    int server_socket;
    struct sockaddr_in server_addr;
    std::vector<std::thread> threads;
    std::map<std::string, std::map<std::string, std::string(*)(Request*, Response*)>> handlers;

    void startServer();
    void stopServer();
    void handleClient(int client_socket);
    std::string processRequest(const std::string& request);
    void joinThreads();
};
