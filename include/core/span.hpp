#ifndef CBB_CORE_SPAN_HPP
#define CBB_CORE_SPAN_HPP

#include <string_view>

#include <cstddef>

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

template<class T>
struct spanned {
public:
    T node;
    span span;

    spanned(T node, class span span): node(node), span(span) {}
    spanned() = default;
};
    
} // namespace core

    
} // namespace cbb



#endif // CBB_CORE_SPAN_HPP