#ifndef CBB_ANALYSIS_LOWERER_HPP
#define CBB_ANALYSIS_LOWERER_HPP

#include "analysis/ids.hpp"

namespace cbb {

namespace sema {

class lowerer {
public:
    lowerer();

private:
    hirid current_hirid_;
};
    
} // namespace sema

    
} // namespace cbb


#endif // CBB_ANALYSIS_LOWERER_HPP