#ifndef CBB_ANALYSIS_ANALYSIS_HPP
#define CBB_ANALYSIS_ANALYSIS_HPP

#include <vector>

#include "core/diagnostics.hpp"
#include "analysis/hir.hpp"

namespace cbb {

namespace sema {

class semantic_analyser {
public:
    semantic_analyser(std::vector<core::diagnostic> &diagnostics);
     
private:
    std::vector<core::diagnostic> &diagnostics;
};


} // namespace sema

    
} // namespace cbb



#endif // CBB_ANALYSIS_ANALYSIS_HPP