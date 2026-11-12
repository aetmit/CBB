#include "analysis/ids.hpp"

namespace cbb {

namespace sema {

localid &localid::operator++() {
    value++;
    return *this;
}

std::ostream &operator<<(std::ostream &os, const localid &id) {
    if (id == localid::error()) {
        os << "<error>";
    } else {
        os << id.value;
    }

    return os;
}

std::ostream &operator<<(std::ostream &os, const hirid &id) {
    os << id.value;

    return os;
}
    
} // namespace sema

    
} // namespace cbb
