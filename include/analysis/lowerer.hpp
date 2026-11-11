#ifndef CBB_ANALYSIS_LOWERER_HPP
#define CBB_ANALYSIS_LOWERER_HPP

#include "analysis/ids.hpp"
#include "analysis/hir.hpp"
#include "parser/ast.hpp"

namespace cbb {

namespace sema {

class lowerer {
public:
    lowerer();

    program lower(const parser::program &ast);

private:
    hirid current_hirid_;
};
    
} // namespace sema

    
} // namespace cbb


#endif // CBB_ANALYSIS_LOWERER_HPP