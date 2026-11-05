#ifndef CBB_CORE_ARENA_HPP
#define CBB_CORE_ARENA_HPP

#include <memory_resource>

#include <cstddef>

namespace cbb {

namespace core {

class arena {
public:
    arena();

    arena(std::size_t initial_capacity);

    void *allocate(std::size_t size, std::size_t alignment);

private:
    static constexpr std::size_t default_initial_capacity = 8 * 1024;

    std::pmr::monotonic_buffer_resource buffer_;
};

} // namespace core

} // namespace cbb


#endif // CBB_CORE_ARENA_HPP