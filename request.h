#include <string>

class Request {
public:
    Request(const std::string& request_text);
    bool is_valid();
private:
    bool valid;
    std::string method;
    std::string path;
};