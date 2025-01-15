#include "server.h"
#include "file.h"
#include <csignal>
#include <iostream>

Server *server;

void end (int sig) {
    delete server;
    exit(sig);
}

std::string get_page(std::string path) {
    File *file = new File();
    if (path == "") {
        path = "index";
    }
    std::string new_path = "./client" + path + ".html";
    std::string file_text = file->fileFromPath(new_path);
    delete file;
    return file_text;
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
    server->get("/", [](std::string path) -> std::string {
        File *file = new File();
        std::string file_text = file->fileFromPath("./client/index.html");
        delete file;
        return file_text;
    });
    server->get("/autor", [](std::string path) -> std::string {
        File *file = new File();
        std::string file_text = file->fileFromPath("./client/autor.html");
        delete file;
        return file_text;
    });
    server->run();

    end(0);

    return 0;
}
