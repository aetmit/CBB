#ifndef CBB_SEMA_IDS_HPP
#define CBB_SEMA_IDS_HPP

#include "core/types.hpp"

namespace cbb {

namespace sema {

struct HirId {
    size_t value;
    explicit HirId(size_t value);
    HirId();
    
    HirId &operator+=(const size_t &other);
    HirId &operator++();
    HirId operator++(int);
};

} // namespace cbb

} // namespace cbb

#endif // CBB_SEMA_IDS_HPP