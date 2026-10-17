#include "core/span.hpp"

namespace cbb {

namespace core {

size_t Span::end() const {
    return offset + length;
}

Span::Span(size_t offset, size_t length): offset(offset), length(length) {}

std::ostream &operator<<(std::ostream &os, const Span &span) {
    os << "Offset: " << span.offset << "\nLength: " << span.length << "\n";
    return os;
}

} // namespace core 

} // namespace cbb