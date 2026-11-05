#include "core/span.hpp"

namespace cbb {

namespace core {

span::span(): offset_(0), length_(0) {}

span::span(std::size_t offset, std::size_t length): offset_(offset), length_(length) {}

std::size_t span::offset() const {
    return offset_;
}

std::size_t span::length() const {
    return length_;
}

bool span::operator==(const span &other) const {
    return offset_ == other.offset() && length_ == other.length();
}

bool span::operator!=(const span &other) const {
    return !(*this == other);
}
    
} // namespace core

    
} // namespace cbb