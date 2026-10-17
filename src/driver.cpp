#include "driver.hpp"

#include "core/fs.hpp"

namespace cbb {

using core::FileReader;
using core::File;

int run(int argc, const char *argv[]) {
    if (argc < 2) {
        return 1;
    }

    FileReader reader(argv[1]);
    File file = reader.readFile();

    return 0;
}

} // namespace cbb