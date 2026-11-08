#ifndef CBB_ANALYSIS_SCOPE_HPP
#define CBB_ANALYSIS_SCOPE_HPP

#include <unordered_map>
#include <string_view>

#include "analysis/ids.hpp"

namespace cbb {

namespace sema {

struct scope {
    scope() = default;

    std::unordered_map<std::string_view, localid> variables;
};
    
} // namespace sema 
    
} // namespace cbb


#endif // CBB_ANALYSIS_SCOPE_HPP