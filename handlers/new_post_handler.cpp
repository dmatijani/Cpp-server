#include "new_post_handler.h"
#include "../file.h"
#include "../html.h"
#include "../time.h"
#include "../data/objava.h"
#include <cstring>

char get_random_char() {
    if (rand() % 2 == 0) {
        return 'A' + rand() % ('Z' - 'A');
    } else {
        return 'a' + rand() % ('z' - 'a');
    }
}

void generate_uuid(char out[50]) {
    char time[30];
    Time::get_time(time);
    bool insert_time = true;
    for (int i = 0; i < 49; i++) {
        if (insert_time) {
            char time_char = time[i];
            if (time_char == '\0' || time_char < '0' || time_char > '9') {
                if (time_char == '\0') {
                    insert_time = false;
                }
                out[i] = get_random_char();
            } else {
                out[i] = time_char;
            }
        } else {
            out[i] = get_random_char();
        }
    }
    out[49] = '\0';
}

void NewPostHandler::handle_new_post(Request* req, Response* res) {
    res->set_content_type("text/html");

    if (req->form_data.size() == 0) {
        delete res;
        res = Response::bad_request();
        return;
    }

    auto title_iter = req->form_data.find("naslov");
    if (title_iter == req->form_data.end()) {
        delete res;
        res = Response::bad_request();
        return;
    }

    auto content_iter = req->form_data.find("sadrzaj");
    if (content_iter == req->form_data.end()) {
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

    Objava objava;
    generate_uuid(objava.uuid);
    std::strncpy(objava.naslov, req->form_data["naslov"].c_str(), sizeof(objava.naslov) - 1);
    objava.naslov[sizeof(objava.naslov) - 1] = '\0';
    std::strncpy(objava.sadrzaj, req->form_data["sadrzaj"].c_str(), sizeof(objava.sadrzaj) - 1);
    objava.sadrzaj[sizeof(objava.sadrzaj) - 1] = '\0';
    std::strncpy(objava.autor, req->form_data["autor"].c_str(), sizeof(objava.autor) - 1);
    objava.autor[sizeof(objava.autor) - 1] = '\0';
    Time::get_time(objava.vrijeme);

    File::write_to_binary_file("./data/objave.bin", objava);

    std::string template_text = File::file_from_path("./client/template.html");
    std::string new_post_text = File::file_from_path("./client/nova_objava.html");
    Html* html = new Html(template_text);
    html->set_title("Prebacujem...")->set_content(new_post_text);
    res->set_status(201)->set_data(html->get_html());
    delete html;
}