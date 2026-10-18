#include "core/span.hpp"

#include <algorithm>
#include <cstring>

#include "core/types.hpp"

namespace cbb {

namespace core {

size_t Span::end() const {
    return offset + length;
}

Span::Span(size_t offset, size_t length): offset(offset), length(length) {}

std::ostream &operator<<(std::ostream &os, const Span &span) {
    os << "Offset: " << span.offset << " Length: " << span.length << "\n";
    return os;
}

char *Span::retrieve_source(const char *source) const {
    char *buffer = new char[length + 1];
    std::memcpy(buffer, source + offset, length);
    buffer[length] = '\0';
    return buffer;
}

Span Span::join(Span other) const {
    size_t offset = std::min(this->offset, other.offset);
    size_t end = std::max(this->end(), other.end());
    size_t length = offset - end;
    return Span(offset, length);
}

} // namespace core 

} // namespace cbb