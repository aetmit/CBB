#include "cbb.hpp"

#include <string>

#include "core/file.hpp"
#include "core/error.hpp"

namespace cbb {

using namespace core;

int run(int argc, const char *argv[]) {
    if (argc < 2) {
        FATAL(LOCATION, "no input files");
    }

    FileReader reader; 

    auto result = reader.readFile(argv[1]);
    
    std::string file;
    if (result) {
        file = *result;
    } else {
        FATAL(LOCATION, "failed to read file");
    }

    return 0;
}

} // namespace cbb