#include "get_posts_handler.h"
#include "../file.h"
#include "../html.h"
#include "../data/objava.h"
#include <vector>
#include <iostream>

#ifndef PAGE_SIZE
#define PAGE_SIZE 10
#endif

std::string add_posts(int page) {
    std::string posts_html = "";
    std::vector<Objava> posts = File::read_from_binary_file<Objava>("./data/objave.bin", page*PAGE_SIZE, PAGE_SIZE);
    for (auto & post : posts) {
        posts_html += "<div>"
                      "<h3>" + std::string(post.naslov) + "</h3>"
                      "<p>Objavljeno: " + std::string(post.vrijeme) + "</p>"
                      "<a href='/objava?id=" + std::string(post.uuid) + "'>Detalji</a>"
                      "</div>";
    }

    return posts_html;
}

std::string add_arrows() {
    return "<button id='prevPageButton'><</button><span id='pageCount'></span><button id='nextPageButton'>></button>";
}

void GetPostsHandler::handle_get_posts(Request* req, Response* res) {
    int page = 0;
    if (req->params.find("page") != req->params.end()) {
        try {
            page = stoi(req->params["page"]);
        } catch (...) {
            page = 0;
        }
    }

    std::string template_text = File::fileFromPath("./client/template.html");
    std::string homepage_text = File::fileFromPath("./client/index.html");
    homepage_text += add_posts(page);
    homepage_text += add_arrows();
    Html* html = new Html(template_text);
    html->set_title("Početna")->set_content(homepage_text);
    res->set_content_type("text/html")->set_status(200)->set_data(html->get_html());
    delete html;
}