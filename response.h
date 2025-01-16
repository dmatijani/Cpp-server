#include <string>

class Response {
public:
    Response();
    Response* set_status(int status);
    Response* set_content_type(std::string content_type);
    Response* set_data(std::string data);
    Response* set_cache_control(std::string cache_control);
    Response* set_expires(std::string expires);
    std::string text();
    static Response* ok();
    static Response* bad_request();
    static std::string not_found_text();
    static std::string bad_request_text();
    void print();
    Response* no_print();
private:
    int status_code;
    std::string content_type;
    std::string data;
    std::string cache_control;
    std::string expires;
    bool allow_print;

    std::string get_status_code_text();
};