#include "core/file.hpp"

#include <fstream>

namespace cbb {

namespace core {

std::optional<std::string> file_reader::read_file(const std::string &path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);

    if (!file) {
        return std::nullopt;
    }

    std::streamsize size = file.tellg();

    file.seekg(0, std::ios::beg);

    std::string buffer;
    buffer.resize(size);

    if (!file.read(&buffer[0], size)) {
        return std::nullopt;
    }

    return buffer;
}

} // namespace core

} // namespace cbb