#include "new_post_handler.h"
#include "../file.h"
#include "../html.h"
#include "../data/objava.h"
#include <cstring>
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>

#include <iostream>

void get_time(char out[30])
{
    time_t sad = time(0);
    tm *ltm = localtime(&sad);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M:%S", ltm);
    std::string time_str = std::string(buffer);
    for (int i = 0; i < 30; i++) {
        out[i] = time_str[i];
    }
}

void generate_uuid(char out[36]) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);

    std::stringstream ss;
    ss << std::hex << std::setw(8) << std::setfill('0') << dis(gen) << dis(gen) << dis(gen) << dis(gen);
    ss << '-';
    ss << std::setw(4) << std::setfill('0') << dis(gen) << dis(gen);
    ss << '-';
    ss << std::setw(4) << std::setfill('0') << dis(gen) << dis(gen);
    ss << '-';
    ss << std::setw(4) << std::setfill('0') << dis(gen) << dis(gen);
    ss << '-';
    ss << std::setw(12) << std::setfill('0') << dis(gen) << dis(gen) << dis(gen) << dis(gen);

    std::string uuid_str = ss.str();
    for (int i = 0; i < 35; i++) {
        out[i] = uuid_str[i];
    }
    out[35] = '\0';
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
    get_time(objava.vrijeme);

    File::write_to_binary_file("./data/objave.bin", objava);

    std::string template_text = File::fileFromPath("./client/template.html");
    std::string new_post_text = File::fileFromPath("./client/nova_objava.html");
    Html* html = new Html(template_text);
    html->set_title("Prebacujem...")->set_content(new_post_text);
    res->set_status(200)->set_data(html->get_html());
    delete html;
}