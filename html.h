#ifndef HTML_H
#define HTML_H

#include <string>

class Html {
public:
    Html(std::string html_template);
    Html* set_template(std::string html_template);
    Html* set_title(std::string title);
    Html* set_content(std::string content);
    std::string get_html();
private:
    std::string html_template;
    std::string title;
    std::string content;
};

#endif