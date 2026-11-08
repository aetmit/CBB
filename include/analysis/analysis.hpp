#ifndef CBB_ANALYSIS_ANALYSIS_HPP
#define CBB_ANALYSIS_ANALYSIS_HPP

#include <vector>

#include "core/diagnostics.hpp"
#include "parser/ast.hpp"
#include "analysis/hir.hpp"

namespace cbb {

namespace sema {

class semantic_analyser {
public:
    semantic_analyser(std::vector<core::diagnostic> &diagnostics);
    
    program analyse(const parser::program &program);
     
private:
    std::vector<core::diagnostic> &diagnostics_;
};


} // namespace sema

    
} // namespace cbb



#endif // CBB_ANALYSIS_ANALYSIS_HPP