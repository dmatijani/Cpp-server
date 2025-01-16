#include "get_post_details_handler.h"
#include "../file.h"
#include "../html.h"
#include "../data/objava.h"
#include "../data/komentar.h"
#include <vector>

std::string get_comments(std::string post_id) {
    std::string comments_html = "";
    std::vector<Komentar> comments = File::read_from_binary_file<Komentar>("./data/" + post_id + ".bin");
    for (auto & comment : comments) {
        std::string komentar_template = File::file_from_path("./client/components/komentar.html");
        Html* comment_html = new Html(komentar_template);
        comment_html->set_placeholder("komentar", std::string(comment.komentar));
        comment_html->set_placeholder("vrijeme", std::string(comment.vrijeme));
        comment_html->set_placeholder("autor", std::string(comment.autor));
        comments_html += comment_html->get_html();
        delete comment_html;
    }

    return comments_html;
}

std::string get_post(std::string uuid) {
    Objava post;

    std::vector<Objava> posts = File::read_from_binary_file<Objava>("./data/objave.bin");
    for (auto & post : posts) {
        if (post.uuid == uuid) {
            std::string objava_template = File::file_from_path("./client/detalji_objave.html");
            Html* post_html = new Html(objava_template);
            post_html->set_placeholder("naslov", std::string(post.naslov));
            post_html->set_placeholder("vrijeme", std::string(post.vrijeme));
            post_html->set_placeholder("autor", std::string(post.autor));
            post_html->set_placeholder("sadrzaj", std::string(post.sadrzaj));
            std::string to_return = post_html->get_html();
            delete post_html;
            return to_return;
        }
    }

    return "<p>Nije pronađena.</p>";
}

void GetPostDetailsHandler::handle_get_post_details(Request* req, Response* res) {
    std::string id = "";
    if (req->params.find("id") == req->params.end()) {
        delete res;
        res = Response::bad_request();
        return;
    }

    id = req->params["id"];

    std::string template_text = File::file_from_path("./client/template.html");
    Html* html = new Html(template_text);
    html->set_title("Objava")->set_content(get_post(id));
    html->set_placeholder("komentari", get_comments(id));
    res->set_content_type("text/html")->set_status(200)->set_data(html->get_html());
    delete html;
}