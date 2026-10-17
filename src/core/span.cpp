#include "core/span.hpp"

namespace cbb {

namespace core {

size_t Span::end() const {
    return offset + length;
}

Span::Span(size_t offset, size_t length): offset(offset), length(length) {}

} // namespace core 

} // namespace cbb