#include "core/fs.hpp"

#include <cstdlib>
#include <cstring>

namespace cbb {

namespace core {

FileReader::FileReader(const char *path) {
    file = std::fopen(path, "rb");
}

FileReader::~FileReader() {
    if (file) {
        fclose(file);
        file = nullptr;
    }
}

const char *FileReader::readFile() {
    std::fseek(file, 0, SEEK_END);

    long length = std::ftell(file);

    std::rewind(file);

    char *buffer = static_cast<char*>(std::malloc(length + 1));

    std::fread(buffer, 1, length, file);

    buffer[length] = '\0';

    return buffer;
}

} // namespace core

} // namespace cbb