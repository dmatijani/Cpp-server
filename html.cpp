#include "html.h"
#include <iostream>

Html::Html(std::string html_template) {
    this->html_template = html_template;
    this->placeholders["title"] = "Stranica";
    this->placeholders["content"] = "";
}

Html* Html::set_template(std::string html_template) {
    this->html_template = html_template;
    return this;
}

Html* Html::set_placeholder(std::string placeholder, std::string content) {
    this->placeholders[placeholder] = content;
    return this;
}

Html* Html::set_title(std::string title) {
    this->placeholders["title"] = title;
    return this;
}

Html* Html::set_content(std::string content) {
    this->placeholders["content"] = content;
    return this;
}

std::string Html::get_html() {
    std::string html = this->html_template;

    for (auto const& placeholder : this->placeholders) {
        std::string placeholder_mark = "[[ " + placeholder.first + " ]]";
        size_t placeholder_start_pos = 0;
        while ((placeholder_start_pos = html.find(placeholder_mark, placeholder_start_pos)) != std::string::npos) {
            html.replace(placeholder_start_pos, placeholder_mark.length(), placeholder.second);
        }
    }

    return html;
}