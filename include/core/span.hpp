#ifndef CBB_CORE_SPAN_HPP
#define CBB_CORE_SPAN_HPP

#include <string_view>
#include <ostream>

#include <cstddef>

#include "core/concepts.hpp"

namespace cbb {

namespace core {

class span {
public:
    span();
    span(std::size_t offset, std::size_t length);

    constexpr std::size_t offset() const {
        return offset_;
    }

    constexpr std::size_t length() const {
        return length_;
    }

    constexpr std::size_t end() const {
        return offset() + length();
    }

    span join(const span &other) const;

    std::string_view retrieve_lexeme(std::string_view source) const; 

    bool operator==(const span &other) const;
    bool operator!=(const span &other) const;
private:
    std::size_t offset_;
    std::size_t length_;
};

std::ostream &operator<<(std::ostream &os, const span &span);

template <class T>
struct spanned {
public:
    T node;
    span span;

    spanned(T node, class span span): node(node), span(span) {}
    spanned() = default;
};

template <class T>
requires core::writable_to_stream<T>
std::ostream &operator<<(std::ostream &os, const spanned<T> &obj) {
    os << "spanned<T> {\nnode: " << obj.node << "\nspan: " << obj.span << "\n}\n";
    return os;
}
    
} // namespace core

    
} // namespace cbb



#endif // CBB_CORE_SPAN_HPP