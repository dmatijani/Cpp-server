#include "request.h"
#include <iostream>

Request::Request(const std::string& request_text) {
    this->valid = true;

    size_t start = request_text.find(" ");
    if (start != std::string::npos) {
        this->valid = false;
    }
    
    size_t end = request_text.find(" ", start + 1);
    if (end != std::string::npos) {
        this->valid = false;
    }

    this->method = request_text.substr(0, start);
    this->path = request_text.substr(start + 1, end - start - 1);

    std::cout << "REQUEST METHOD: " << this->method << std::endl;
    std::cout << "REQUEST PATH: " << this->path << std::endl;
}

bool Request::is_valid() {
    return this->valid;
}