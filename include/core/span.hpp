#ifndef CBB_CORE_SPAN_HPP
#define CBB_CORE_SPAN_HPP

#include <cstddef>

namespace cbb {

namespace core {

class span {
public:
    span();
    span(std::size_t offset, std::size_t length);

    std::size_t offset() const;
    std::size_t length() const;

    bool operator==(const span &other) const;
    bool operator!=(const span &other) const;
private:
    std::size_t offset_;
    std::size_t length_;
};

template<typename T>
struct spanned {
public:
    T node;
    span span;

    spanned(T node, class span span): node(node), span(span) {}
};
    
} // namespace core

    
} // namespace cbb



#endif // CBB_CORE_SPAN_HPP