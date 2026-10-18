#include "core/span.hpp"

#include <cstring>

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

char *Span::retrieve_source(const char *source) const {
    char *buffer = new char[length + 1];
    std::memcpy(buffer, source + offset, length);
    buffer[length] = '\0';
    return buffer;
}

} // namespace core 

} // namespace cbb