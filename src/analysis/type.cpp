#include "analysis/type.hpp"

namespace cbb {

namespace sema {

bool type_info::operator==(const type_info &other) const {
    return type == other.type && category == other.category;
}

bool type_info::operator!=(const type_info &other) const {
    return !(*this == other);
}
    
} // namespace sema

} // namespace cbb
