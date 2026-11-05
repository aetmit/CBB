#include "cbb.hpp"

#include <string>

#include "core/file.hpp"
#include "core/error.hpp"

namespace cbb {

int run(int argc, const char *argv[]) {
    if (argc < 2) {
        FATAL("no input files");
    }

    core::file_reader reader; 

    auto result = reader.read_file(argv[1]);
    
    std::string file;
    if (result) {
        file = *result;
    } else {
        FATAL("failed to read file");
    }

    return 0;
}

} // namespace cbb