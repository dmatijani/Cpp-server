#include "file.h"
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

template <typename T>
void File::write_to_binary_file(const std::string& path, const T& data) {
    std::ofstream file(path, std::ios::binary | std::ios::app);
    if (!file) {
        std::cerr << "Ne mogu otvoriti datoteku za pisanje: " << path << std::endl;
        return;
    }
    file.write(reinterpret_cast<const char*>(&data), sizeof(T));
    file.close();
}

template <typename T>
std::vector<T> File::read_from_binary_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Ne mogu otvoriti datoteku za čitanje: " << path << std::endl;
        return {};
    }

    std::vector<T> data;
    T temp;

    while (file.read(reinterpret_cast<char*>(&temp), sizeof(T))) {
        data.push_back(temp);
    }

    file.close();
    return data;
}