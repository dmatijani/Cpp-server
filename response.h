#include <string>

class Response {
public:
    Response();
    Response* set_status(int status);
    Response* set_content_type(std::string content_type);
    Response* set_data(std::string data);
    std::string text();
    static Response* ok();
    static Response* bad_request();
private:
    int status_code;
    std::string content_type;
    std::string data;

    std::string get_status_code_text();
};