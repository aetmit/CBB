#ifndef CBB_CORE_FILE_HPP
#define CBB_CORE_FILE_HPP

#include <string>
#include <optional>

namespace cbb {

namespace core {

class file_reader {
public:
    std::optional<std::string> read_file(const std::string &path);
};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_FILE_HPP