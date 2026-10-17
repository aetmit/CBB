#include "core/span.hpp"

namespace cbb {

namespace core {

size_t Span::end() const {
    return offset + length;
}

} // namespace core 

} // namespace cbb