#include "sema/ids.hpp"

namespace cbb {

namespace sema {

HirId::HirId(size_t value): value(value) {}

HirId &HirId::operator+=(const size_t &other) {
    value += other;
    return *this;
}

HirId &HirId::operator++() {
    *this += 1;
    return *this;
}

HirId HirId::operator++(int) {
    HirId tmp = *this;
    ++(*this);
    return tmp;
}

} // namespace sema

} // namespace cbb