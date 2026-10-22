#include "core/arena.hpp"

namespace cbb {

namespace core {

void *arena::alloc(size_t size, size_t alignment) {
    return resource->allocate(size, alignment);
}

arena::~arena() {
    reset();
}

arena::arena(): resource(std::make_unique<std::pmr::monotonic_buffer_resource>()) {}

void arena::reset() {
    for (auto it = destructors.rbegin(); it != destructors.rend(); ++it) {
        it->destroy(it->object);
    }

    destructors.clear();

    if (resource) {
        resource.reset(); 
    }

    resource = std::make_unique<std::pmr::monotonic_buffer_resource>();
}

arena::arena(arena &&other) noexcept : resource(std::move(other.resource)), destructors(std::move(other.destructors)) {}

arena &arena::operator=(arena &&other) noexcept {
    if (this == &other) {
        return *this;
    }

    reset();

    resource = std::move(other.resource);
    destructors = std::move(other.destructors);

    return *this;
}

} // namespace core 

} // namespace cbb