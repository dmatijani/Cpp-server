#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <vector>
#include <map>

class Request {
public:
    std::string request_time;
    std::string client_ip;
    std::string method;
    std::string path;
    std::string content_type;
    std::string data;
    std::vector<std::string> accept;
    std::map<std::string, std::string> params;
    std::map<std::string, std::string> form_data;

    Request(const std::string& request_text, std::string ip);
    bool is_valid();
    void print();
    Request* no_print();
private:
    bool valid;
    bool allow_print;

    std::string get_time();
};

#endif