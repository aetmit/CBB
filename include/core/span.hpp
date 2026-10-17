#ifndef CBB_CORE_SPAN_HPP
#define CBB_CORE_SPAN_HPP

#include <iostream>

#include "core/types.hpp"

namespace cbb {

namespace core {

struct Span {
    size_t offset;
    size_t length;

    Span(size_t offset, size_t length);
    size_t end() const;

    friend std::ostream &operator<<(std::ostream &os, const Span &span);
};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_SPAN_HPP