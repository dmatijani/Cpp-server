#include "file.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string File::fileFromPath(std::string path) {
    try {
        std::fstream file(path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    } catch (...) {
        return "";
    }
}