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

File FileReader::readFile() {
    std::fseek(file, 0, SEEK_END);

    long length = std::ftell(file);

    std::rewind(file);

    char *buffer = static_cast<char*>(std::malloc(length + 1));

    std::fread(buffer, 1, length, file);

    buffer[length] = '\0';

    return File(buffer);
}

File::File(const char *contents): data(contents) {}

File::~File() {
    std::free(const_cast<void*>(static_cast<const void*>(data)));
}

File::File(): data(nullptr) {}

const char *File::contents() const {
    return data;
}

} // namespace core

} // namespace cbb