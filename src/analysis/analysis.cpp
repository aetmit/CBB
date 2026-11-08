#include "analysis/analysis.hpp"

#include "analysis/resolution.hpp"
#include "analysis/context.hpp"

namespace cbb {

namespace sema {

semantic_analyser::semantic_analyser(std::vector<core::diagnostic> &diagnostics): diagnostics_(diagnostics) {}

program semantic_analyser::analyse(const parser::program &program) {

    context context(diagnostics_);

    resolver resolver;
    resolver.resolve(program, context);
    
    
}
    
} // namespace sema

    
} // namespace cbb
