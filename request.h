#include <string>
#include <vector>
#include <map>

class Request {
public:
    std::string method;
    std::string path;
    std::vector<std::string> accept;
    std::map<std::string, std::string> params;

    Request(const std::string& request_text);
    bool is_valid();
private:
    bool valid;
};