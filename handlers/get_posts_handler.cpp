#include "get_posts_handler.h"
#include "../file.h"
#include "../html.h"
#include "../data/objava.h"
#include <vector>
#include <iostream>

std::string add_posts() {
    std::string posts_html = "";
    std::vector<Objava> posts = File::read_from_binary_file<Objava>("./data/objave.bin");
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
    return "<p>Ovdje će biti strjelice!</p>";
}

void GetPostsHandler::handle_get_posts(Request* req, Response* res) {
    std::string template_text = File::fileFromPath("./client/template.html");
    std::string homepage_text = File::fileFromPath("./client/index.html");
    homepage_text += add_posts();
    homepage_text += add_arrows();
    Html* html = new Html(template_text);
    html->set_title("Početna")->set_content(homepage_text);
    res->set_content_type("text/html")->set_status(200)->set_data(html->get_html());
    delete html;
}