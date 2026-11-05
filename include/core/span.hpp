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
private:
    std::size_t offset_;
    std::size_t length_;
};
    
} // namespace core

    
} // namespace cbb



#endif // CBB_CORE_SPAN_HPP