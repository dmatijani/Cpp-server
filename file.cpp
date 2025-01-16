#include "file.h"
#include <sstream>

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