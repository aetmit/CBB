#ifndef CBB_SEMA_LOWERER_HPP
#define CBB_SEMA_LOWERER_HPP

#include "sema/ids.hpp"

namespace cbb {

namespace sema {

class Lowerer {
public:
    Lowerer();
    
private:
    HirId currentHirId;
};

} // namespace sema

} // namespace cbb

#endif // CBB_SEMA_LOWERER_HPP