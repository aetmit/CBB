#include "cbb.hpp"

#include <string>
#include <vector>
#include <utility>

#include "core/file.hpp"
#include "core/error.hpp"
#include "core/diagnostics.hpp"
#include "parser/parser.hpp"
#include "parser/ast.hpp"
#include "analysis/analysis.hpp"
#include "analysis/hir.hpp"

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

    if (!diagnostics.empty()) {
        core::diagnostic_formatter formatter(file);
        for (auto it = diagnostics.begin(); it != diagnostics.end(); ++it) {
            std::cout << formatter.format(*it) << "\n";
        }

        return 1;
    }

    parser::ast_formatter ast_formatter;
    auto formatted_ast = ast_formatter.format(ast);
    std::cout << "AST:\n" << formatted_ast << "\n\n\n\n\n";

    sema::semantic_analyser analyser(diagnostics);
    auto res = analyser.analyse(ast);

    if (!diagnostics.empty()) {
        core::diagnostic_formatter formatter(file);
        for (auto it = diagnostics.begin(); it != diagnostics.end(); ++it) {
            std::cout << formatter.format(*it) << "\n";
        }

        return 1;
    } 

    auto hir = std::move(*res);

    sema::hir_formatter hir_formatter;
    auto formatted_hir = hir_formatter.format(hir);
    std::cout << "HIR:\n" << formatted_hir << "\n\n";

    return 0;
}

} // namespace cbb