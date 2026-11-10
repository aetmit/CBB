#ifndef CBB_ANALYSIS_CONTEXT_HPP
#define CBB_ANALYSIS_CONTEXT_HPP

#include <vector>
#include <unordered_map>

#include "core/diagnostics.hpp"
#include "analysis/scope.hpp"
#include "analysis/ids.hpp"
#include "analysis/type.hpp"
#include "parser/ast.hpp"

namespace cbb {

namespace sema {

struct context {
    context(std::vector<core::diagnostic> &diagnostics);

    std::vector<scope> scopes;

    std::vector<core::diagnostic> &diagnostics;

    std::unordered_map<parser::nodeid, localid> variables;
    std::unordered_map<localid, type_info> var_types;
    std::unordered_map<parser::nodeid, type_info> types;
};
    
} // namespace sema

    
} // namespace cbb


#endif // CBB_ANALYSIS_CONTEXT_HPP