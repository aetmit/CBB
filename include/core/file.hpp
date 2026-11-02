#ifndef CBB_CORE_FILE_HPP
#define CBB_CORE_FILE_HPP

#include <string>
#include <optional>

namespace cbb {

namespace core {

class FileReader {
public:
    std::optional<std::string> readFile(const std::string &path);
};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_FILE_HPP