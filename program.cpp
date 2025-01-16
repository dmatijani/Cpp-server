#include "file.h"
#include "html.h"
#include "server/server.h"
#include "handlers/new_post_handler.h"
#include "handlers/get_posts_handler.h"
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

    signal(SIGINT, end);

    server = new Server("127.0.0.1", port);

    server->get("/js", [](Request* req, Response* res) {
        req->no_print();
        std::string file_text = File::fileFromPath("./client/js/app.js");
        res->set_content_type("application/javascript")->set_status(200)->no_print()->set_data(file_text);
    });
    server->get("/css", [](Request* req, Response* res) {
        req->no_print();
        std::string file_text = File::fileFromPath("./client/styles/style.css");
        res->set_content_type("text/css")->set_status(200)->no_print()->set_data(file_text);
    });

    server->get("/", GetPostsHandler::handle_get_posts);
    server->get("/autor", [](Request* req, Response* res) {
        std::string template_text = File::fileFromPath("./client/template.html");
        std::string author_text = File::fileFromPath("./client/autor.html");
        Html* html = new Html(template_text);
        html->set_title("O autoru")->set_content(author_text);
        res->set_content_type("text/html")->set_status(200)->set_data(html->get_html());
        delete html;
    });
    server->get("/objavi", [](Request* req, Response* res) {
        std::string template_text = File::fileFromPath("./client/template.html");
        std::string post_text = File::fileFromPath("./client/objavi.html");
        Html* html = new Html(template_text);
        html->set_title("Nova objava")->set_content(post_text);
        res->set_content_type("text/html")->set_status(200)->set_data(html->get_html());
        delete html;
    });
    server->post("/novaObjava", NewPostHandler::handle_new_post);

    server->run();

    end(0);

    return 0;
}
