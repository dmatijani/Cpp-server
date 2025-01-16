#ifndef FILE_H
#define FILE_H

#include <string>
#include <fstream>
#include <vector>

class File {
public:
    static std::string fileFromPath(std::string path);
    template <typename T>
    static void write_to_binary_file(const std::string& path, const T& data);

    template <typename T>
    static std::vector<T> read_from_binary_file(const std::string& path);
};

#endif

#include <iostream>

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