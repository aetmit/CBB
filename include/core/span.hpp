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
    Span() = default;
    size_t end() const;
    // must be freed by caller
    char *retrieve_source(const char *source) const;

    friend std::ostream &operator<<(std::ostream &os, const Span &span);
};

template<typename T>
struct Spanned {
    T node;
    Span span;
    Spanned(T node, Span span): node(node), span(span) {}
};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_SPAN_HPP