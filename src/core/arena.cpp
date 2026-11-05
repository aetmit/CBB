#include "core/arena.hpp"

namespace cbb {

namespace core {

arena::arena(): buffer_(default_initial_capacity) {}

arena::arena(std::size_t initial_capacity): buffer_(initial_capacity) {}

void *arena::allocate(std::size_t size, std::size_t alignment) {
    return buffer_.allocate(size, alignment);
}

} // namespace core

} // namespace cbb