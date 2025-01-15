#include "response.h"
#include <sstream>
#include <iostream>

Response::Response() {
    this->status_code = 200;
    this->content_type = "text/plain";
    this->data = "";
}

Response* Response::set_status(int status) {
    this->status_code = status;
    return this;
}

Response* Response::set_content_type(std::string content_type) {
    this->content_type = content_type;
    return this;
}

Response* Response::set_data(std::string data) {
    this->data = data;
    return this;
}

std::string Response::text() {
    std::stringstream stream;
    stream << "HTTP/1.1 " << this->status_code << " " << this->get_status_code_text();
    stream << "\r\n";
    stream << "Content-Type: " << this->content_type;
    stream << "\r\n\r\n";
    stream << this->data;
    return stream.str();
}

Response* Response::ok() {
    Response* response = new Response();
    response->set_status(200)->set_content_type("text/html");
    return response;
}

Response* Response::bad_request() {
    Response* response = new Response();
    response->set_status(400)->set_content_type("text/html")->set_data("Invalid request.");
    return response;
}

std::string Response::get_status_code_text() {
    switch (this->status_code) {
        case 200:
            return "OK";
        case 201:
            return "Created";
        case 400:
            return "Bad Request";
        case 401:
            return "Unauthorized";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        default:
        return "";
    }
}

std::string Response::not_found_text() {
    Response* not_found_response = Response::ok();
    not_found_response->set_content_type("text/html");
    not_found_response->set_data("<!DOCTYPE html><html lang='en'><p>Nije pronadena stranica koju trazite.</p></html>");
    std::string not_found_text = not_found_response->text();
    delete not_found_response;
    return not_found_text;
}

std::string Response::bad_request_text() {
    Response* bad_request_response = Response::bad_request();
    std::string bad_request_text = bad_request_response->text();
    delete bad_request_response;
    return bad_request_text;
}