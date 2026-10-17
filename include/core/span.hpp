#ifndef CORE_SPAN_HPP
#define CORE_SPAN_HPP

#include "core/types.hpp"

namespace cbb {

struct Span {
    size_t offset;
    size_t length;
};

} // namespace cbb

#endif // CORE_SPAN_HPP