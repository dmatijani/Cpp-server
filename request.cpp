#include "request.h"
#include <iostream>
#include <sstream>

Request::Request(const std::string& request_text, std::string ip) {
    this->valid = true;

    this->content_type = "";
    this->request_time = get_time();
    this->client_ip = ip;

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
        size_t accept_end_smaller = request_text.find(";q=", accept_start);
        if (accept_end_smaller != std::string::npos && accept_end_smaller < accept_end && accept_end_smaller > accept_start) {
            accept_end = accept_end_smaller;
        }
        std::string accept_value = request_text.substr(accept_start + 8, accept_end - accept_start - 8);

        std::stringstream ss(accept_value);
        std::string mime_type;
        while (std::getline(ss, mime_type, ',')) {
            accept.push_back(mime_type);
        }
    }

    size_t content_type_start = request_text.find("Content-Type: ");
    if (content_type_start != std::string::npos) {
        size_t content_type_end = request_text.find("\r\n", content_type_start);
        this->content_type = request_text.substr(content_type_start + 14, content_type_end - content_type_start - 14);
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

    size_t data_start = request_text.find("\r\n\r\n");
    if (data_start != std::string::npos) {
        this->data = request_text.substr(data_start + 4);
        if (this->content_type == "application/x-www-form-urlencoded") {
            std::stringstream ss(this->data);
            std::string data;
            while (std::getline(ss, data, '&')) {
                size_t equal_pos = data.find("=");
                if (equal_pos != std::string::npos) {
                    std::string key = data.substr(0, equal_pos);
                    std::string value = data.substr(equal_pos + 1);
                    form_data[key] = value;
                }
            }
        }
    }

    this->print();
}

void Request::print() {
    std::cout << "(" << this->request_time << ") " << "ZAHTJEV DOŠAO SA: " << this->client_ip << std::endl;
    std::cout << "(" << this->request_time << ") " <<  "METHOD: " << this->method << std::endl;
    std::cout << "(" << this->request_time << ") " <<  "PATH: " << this->path << std::endl;
    if (this->content_type != "") std::cout << "(" << this->request_time << ") " <<  "CONTENT-TYPE: " << this->content_type << std::endl;
    if (this->data != "") std::cout << "(" << this->request_time << ") " <<  "DATA: " << this->data << std::endl;

    if (!this->accept.empty()) {
        std::cout << "(" << this->request_time << ") " <<  "ACCEPT: ";
        for (const auto& mime : this->accept) {
            std::cout << mime << " ";
        }
        std::cout << std::endl;
    }

    if (!this->params.empty()) {
        std::cout << "(" << this->request_time << ") " <<  "PARAMS:" << std::endl;
        for (const auto& [key, value] : this->params) {
            std::cout << "  " << key << ": " << value << std::endl;
        }
    }

    if (!this->form_data.empty()) {
        std::cout << "(" << this->request_time << ") " <<  "FORM DATA:" << std::endl;
        for (const auto& [key, value] : this->form_data) {
            std::cout << " -" << key << ": " << value << std::endl;
        }
    }

    std::cout << "------------------------------" << std::endl;
}


bool Request::is_valid() {
    return this->valid;
}

#include <chrono>
#include <iomanip>
#include <sstream>

std::string Request::get_time() {
    auto now = std::chrono::system_clock::now();
    
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&in_time_t), "%H:%M:%S")
        << "." << std::setw(3) << std::setfill('0') << milliseconds.count();
    
    return oss.str();
}
