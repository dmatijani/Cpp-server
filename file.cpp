#include "file.h"
#include <sstream>
#include <iostream>

std::mutex File::write_mutex;

std::string File::file_from_path(std::string path) {
    try {
        std::fstream file(path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    } catch (...) {
        return "";
    }
}