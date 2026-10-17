#ifndef CBB_CORE_SPAN_HPP
#define CBB_CORE_SPAN_HPP

#include "core/types.hpp"

namespace cbb {

namespace core {

struct Span {
    size_t offset;
    size_t length;

    size_t end() const;
};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_SPAN_HPP