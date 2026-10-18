#include "sema/analysis.hpp"

namespace cbb {

namespace sema {

SemanticAnalyser::SemanticAnalyser(std::vector<core::Diagnostic> *diagnostics): diagnostics(diagnostics) {}

SemanticAnalyser::~SemanticAnalyser() {
    diagnostics = nullptr;
}

AnalysisCtx::AnalysisCtx(std::vector<core::Diagnostic> *diagnostics): diagnostics(diagnostics) {}

AnalysisCtx::~AnalysisCtx() {
    diagnostics = nullptr;
}

std::optional<Program> SemanticAnalyser::analyse(parser::Program ast) {
    
}

} // namespace sema

} // namespace cbb