#ifndef CBB_ANALYSIS_FUNCTION_INFO_HPP
#define CBB_ANALYSIS_FUNCTION_INFO_HPP

#include <vector>

#include "analysis/type.hpp"

namespace cbb {

namespace sema {

struct signature {
    type return_type;
    std::vector<type> param_types;

    signature(type return_type, std::vector<type> param_types);

    bool operator==(const signature&) const = default;
};

struct function_info {
    signature sig;
    bool defined;

    function_info(signature sig, bool defined);
};
    
} // namespace sema

    
} // namespace cbb


#endif // CBB_ANALYSIS_FUNCTION_INFO_HPP