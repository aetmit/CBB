#include "core/span.hpp"

#include <algorithm>

namespace cbb {

namespace core {

span::span(): offset_(0), length_(0) {}

span::span(std::size_t offset, std::size_t length): offset_(offset), length_(length) {}

std::string_view span::retrieve_lexeme(std::string_view source) const {
    return source.substr(offset_, length_);
}

span span::join(const span &other) const {
    std::size_t new_offset = std::min(offset_, other.offset());
    std::size_t new_end = std::max(end(), other.end());
    std::size_t new_length = new_end - new_offset; 

    return { new_offset, new_length };
}

bool span::operator==(const span &other) const {
    return offset_ == other.offset() && length_ == other.length();
}

bool span::operator!=(const span &other) const {
    return !(*this == other);
}
    
} // namespace core

    
} // namespace cbb