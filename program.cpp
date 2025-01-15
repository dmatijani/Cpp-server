#include "server.h"
#include "file.h"
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

    server->get("/js", [](Request* req, Response* res) {
        File *file = new File();
        std::string file_text = file->fileFromPath("./client/js/app.js");
        delete file;
        res->set_content_type("application/javascript")->set_status(200)->set_data(file_text);
    });
    server->get("/css", [](Request* req, Response* res) {
        File *file = new File();
        std::string file_text = file->fileFromPath("./client/styles/style.css");
        delete file;
        res->set_content_type("text/css")->set_status(200)->set_data(file_text);
    });

    server->get("/", [](Request* req, Response* res) {
        File *file = new File();
        std::string file_text = file->fileFromPath("./client/index.html");
        delete file;
        res->set_content_type("text/html")->set_status(200)->set_data(file_text);
    });
    server->get("/autor", [](Request* req, Response* res) {
        File *file = new File();
        std::string file_text = file->fileFromPath("./client/autor.html");
        delete file;
        res->set_content_type("text/html")->set_status(200)->set_data(file_text);
    });
    server->run();

    end(0);

    return 0;
}
