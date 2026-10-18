#ifndef CBB_SEMA_ANALYSIS_HPP
#define CBB_SEMA_ANALYSIS_HPP

#include <vector>
#include <unordered_map>
#include <optional>

#include "core/diagnostics.hpp"
#include "parse/ast.hpp"
#include "sema/types.hpp"
#include "sema/hir.hpp"

namespace cbb {

namespace sema {

class SemanticAnalyser {
public:
    SemanticAnalyser(std::vector<core::Diagnostic> *diagnostics);
    ~SemanticAnalyser();
    
    std::optional<Program> analyse(parser::Program ast);

private:
    // non owning
    std::vector<core::Diagnostic> *diagnostics;
};

struct AnalysisCtx {
    std::unordered_map<parser::NodeId, Type> types;

    std::vector<core::Diagnostic> *diagnostics;

    AnalysisCtx(std::vector<core::Diagnostic> *diagnostics);
    ~AnalysisCtx();
};

} // namespace sema

} // namespace cbb

#endif // CBB_SEMA_ANALYSIS_HPP