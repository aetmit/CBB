#include "analysis/type.hpp"

namespace cbb {

namespace sema {

bool type_info::operator==(const type_info &other) const {
    return type == other.type && category == other.category;
}

bool type_info::operator!=(const type_info &other) const {
    return !(*this == other);
}

std::ostream &operator<<(std::ostream &os, const type &type) {
    switch (type) {
        case type::bool_:
            os << "bool";
            break;
        case type::int_:
            os << "int";
            break;
        case type::error:
            os << "error";
            break;
    }
    
    return os;
}
    
} // namespace sema

} // namespace cbb
