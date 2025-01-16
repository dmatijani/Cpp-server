#include "get_posts_handler.h"
#include "../file.h"
#include "../html.h"
#include "../data/objava.h"
#include <vector>
#include <iostream>

#ifndef PAGE_SIZE
#define PAGE_SIZE 5
#endif

std::string get_posts(int page_number) {
    std::string posts_html = "";
    std::vector<Objava> posts = File::read_from_binary_file<Objava>("./data/objave.bin", page_number*PAGE_SIZE, PAGE_SIZE);
    for (auto & post : posts) {
        std::string objava_template = File::file_from_path("./client/components/objava.html");
        Html* post_html = new Html(objava_template);
        post_html->set_placeholder("naslov", std::string(post.naslov));
        post_html->set_placeholder("vrijeme", std::string(post.vrijeme));
        post_html->set_placeholder("autor", std::string(post.autor));
        post_html->set_placeholder("putanja", "/objava?id=" + std::string(post.uuid));
        posts_html += post_html->get_html();
        delete post_html;
    }

    return posts_html;
}

std::string get_arrows() {
    return File::file_from_path("./client/components/strelice.html");
}

void GetPostsHandler::handle_get_posts(Request* req, Response* res) {
    int page_number = 0;
    if (req->params.find("page") != req->params.end()) {
        try {
            page_number = stoi(req->params["page"]);
        } catch (...) {
            page_number = 0;
        }
    }

    std::string template_text = File::file_from_path("./client/template.html");
    std::string homepage_text = File::file_from_path("./client/index.html");
    Html* html = new Html(template_text);
    html->set_title("Početna")->set_content(homepage_text);
    html->set_placeholder("posts", get_posts(page_number));
    html->set_placeholder("strelice", get_arrows());
    res->set_content_type("text/html")->set_status(200)->set_data(html->get_html());
    delete html;
}