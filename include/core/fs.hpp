#ifndef CORE_FS_HPP
#define CORE_FS_HPP

#include <cstdio>

namespace cbb {

namespace core {

class FileReader {
public:
    explicit FileReader(const char *path);
    ~FileReader();
    const char *readFile();

private:
    FILE *file;
};

} // namespace core

} // namespace cbb

#endif // CORE_FS_HPP