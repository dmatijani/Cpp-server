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