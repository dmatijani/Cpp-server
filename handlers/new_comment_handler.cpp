#include "new_comment_handler.h"
#include "../file.h"
#include "../data/komentar.h"
#include <cstring>

#include <iostream>

void NewCommentHandler::handle_new_comment(Request* req, Response* res) {
    res->set_content_type("text/html");
    std::cout << "Došao sam ovdje" << std::endl;

    if (req->form_data.size() == 0) {
        delete res;
        res = Response::bad_request();
        return;
    }

    std::string post_id = "";
    if (req->params.find("id") == req->params.end()) {
        delete res;
        res = Response::bad_request();
        return;
    }

    post_id = req->params["id"];

    auto comment_iter = req->form_data.find("komentar");
    if (comment_iter == req->form_data.end()) {
        delete res;
        res = Response::bad_request();
        return;
    }

    auto author_iter = req->form_data.find("autor");
    if (author_iter == req->form_data.end()) {
        delete res;
        res = Response::bad_request();
        return;
    }

    Komentar komentar;
    std::strncpy(komentar.komentar, req->form_data["komentar"].c_str(), sizeof(komentar.komentar) - 1);
    komentar.komentar[sizeof(komentar.komentar) - 1] = '\0';
    std::strncpy(komentar.autor, req->form_data["autor"].c_str(), sizeof(komentar.autor) - 1);
    komentar.autor[sizeof(komentar.autor) - 1] = '\0';

    File::write_to_binary_file("./data/" + post_id + ".bin", komentar);

    res->set_status(201)->set_data("Komentar spremljen!");
}