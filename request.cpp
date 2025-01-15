#include "request.h"
#include <iostream>
#include <sstream>

Request::Request(const std::string& request_text) {
    std::cout << request_text << std::endl;

    this->valid = true;

    size_t start = request_text.find(" ");
    if (start == std::string::npos) {
        this->valid = false;
        return;
    }
    
    size_t end = request_text.find(" ", start + 1);
    if (end == std::string::npos) {
        this->valid = false;
        return;
    }

    this->method = request_text.substr(0, start);
    this->path = request_text.substr(start + 1, end - start - 1);

    size_t accept_start = request_text.find("Accept: ");
    if (accept_start != std::string::npos) {
        size_t accept_end = request_text.find("\r\n", accept_start);
        std::string accept_value = request_text.substr(accept_start + 8, accept_end - accept_start - 8);

        std::stringstream ss(accept_value);
        std::string mime_type;
        while (std::getline(ss, mime_type, ',')) {
            accept.push_back(mime_type);
        }
    }

    size_t query_start = this->path.find("?");
    if (query_start != std::string::npos) {
        std::string query_string = this->path.substr(query_start + 1);

        std::stringstream ss(query_string);
        std::string param;
        while (std::getline(ss, param, '&')) {
            size_t equal_pos = param.find("=");
            if (equal_pos != std::string::npos) {
                std::string key = param.substr(0, equal_pos);
                std::string value = param.substr(equal_pos + 1);
                params[key] = value;
            }
        }

        this->path = this->path.substr(0, query_start);
    }
}

bool Request::is_valid() {
    return this->valid;
}