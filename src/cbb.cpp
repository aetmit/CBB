#include "cbb.hpp"

#include <string>
#include <vector>

#include "core/file.hpp"
#include "core/error.hpp"
#include "core/diagnostics.hpp"
#include "parser/parser.hpp"

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

    std::vector<core::diagnostic> diagnostics;

    parser::parser parser(file, diagnostics);
    auto ast = parser.parse();
    std::cout << ast;

    return 0;
}

} // namespace cbb