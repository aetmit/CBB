#include "analysis/analysis.hpp"

#include "analysis/resolution.hpp"
#include "analysis/context.hpp"
#include "analysis/type_checker.hpp"
#include "analysis/lowerer.hpp"
#include "analysis/hir.hpp"

namespace cbb {

namespace sema {

semantic_analyser::semantic_analyser(std::vector<core::diagnostic> &diagnostics): diagnostics_(diagnostics) {}

std::optional<program> semantic_analyser::analyse(const parser::program &program) {

    if (!diagnostics_.empty()) {
        return std::nullopt;
    }

    context context(diagnostics_);

    resolver resolver;
    resolver.resolve(program, context);

    if (!diagnostics_.empty()) {
        return std::nullopt;
    }
    
    type_checker type_checker;
    type_checker.check(program, context);

    if (!diagnostics_.empty()) {
        return std::nullopt;
    }

    lowerer lowerer;
    sema::program hir = lowerer.lower(program, context);

    return hir;
}
    
} // namespace sema

    
} // namespace cbb
