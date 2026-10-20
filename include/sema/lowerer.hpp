#ifndef CBB_SEMA_LOWERER_HPP
#define CBB_SEMA_LOWERER_HPP

#include "parse/ast.hpp"
#include "sema/ids.hpp"
#include "sema/hir.hpp"

namespace cbb {

namespace sema {

struct AnalysisCtx;

class Lowerer {
public:
    Lowerer();
    
    Program lower(parser::Program program, AnalysisCtx *ctx);
private:
    HirId currentHirId;
};

} // namespace sema

} // namespace cbb

#endif // CBB_SEMA_LOWERER_HPP