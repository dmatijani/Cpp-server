#ifndef HTML_H
#define HTML_H

#include <string>
#include <map>

class Html {
public:
    Html(std::string html_template = "[[ content ]]");
    Html* set_template(std::string html_template);
    Html* set_placeholder(std::string placeholder, std::string content);
    Html* set_title(std::string title);
    Html* set_content(std::string content);
    std::string get_html();
private:
    std::map<std::string, std::string> placeholders;
    std::string html_template;
};

#endif