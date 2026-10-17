#ifndef CORE_FS_HPP
#define CORE_FS_HPP

#include <cstdio>

namespace cbb {

namespace core {

// manages contents for you
class File {
public:
    File();
    explicit File(const char *contents);
    ~File();
    const char *contents() const;
private:
    const char *data;
};

class FileReader {
public:
    explicit FileReader(const char *path);
    ~FileReader();
    File readFile();

private:
    FILE *file;
};

} // namespace core

} // namespace cbb

#endif // CORE_FS_HPP