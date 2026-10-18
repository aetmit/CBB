#include "driver.hpp"

#include <vector>

#include "core/fs.hpp"
#include "core/diagnostics.hpp"
#include "parse/ast.hpp"
#include "parse/parser.hpp"

namespace cbb {

using core::FileReader;
using core::File;

int run(int argc, const char *argv[]) {
    if (argc < 2) {
        return 1;
    }

    FileReader reader(argv[1]);
    File file = reader.readFile();

    std::vector<core::Diagnostic> diagnostics;
    parser::Parser parser(file.contents(), &diagnostics);
    parser::Program program = parser.parse();

    return 0;
}

} // namespace cbb