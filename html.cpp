#include "html.h"

Html::Html(std::string html_template) {
    this->html_template = html_template;
    this->title = "Stranica";
    this->content = "";
}

Html* Html::set_template(std::string html_template) {
    this->html_template = html_template;
    return this;
}

Html* Html::set_title(std::string title) {
    this->title = title;
    return this;
}

Html* Html::set_content(std::string content) {
    this->content = content;
    return this;
}

std::string Html::get_html() {
    std::string html = this->html_template;
    size_t title_start_pos = 0;
    std::string title_placeholder = "[[ title ]]";
    std::string content_placeholder = "[[ content ]]";
    while ((title_start_pos = html.find(title_placeholder, title_start_pos)) != std::string::npos) {
        html.replace(title_start_pos, title_placeholder.length(), this->title);
    }

    size_t content_start_pos = html.find(content_placeholder);
    if (content_start_pos != std::string::npos) {
        html.replace(content_start_pos, content_placeholder.length(), this->content);
    }

    return html;
}