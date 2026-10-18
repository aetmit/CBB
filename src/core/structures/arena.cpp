#include "core/structures/arena.hpp"

namespace cbb {

namespace core {

void *Arena::alloc(size_t size, size_t alignment) {
    return resource->allocate(size, alignment);
}

Arena::~Arena() {
    reset();
}

Arena::Arena(): resource(std::make_unique<std::pmr::monotonic_buffer_resource>()) {}

void Arena::reset() {
    for (auto it = destructors.rbegin(); it != destructors.rend(); ++it) {
        it->destroy(it->object);
    }

    destructors.clear();
    resource.release();
}

Arena::Arena(Arena &&other) noexcept : resource(std::move(other.resource)), destructors(std::move(other.destructors)) {}

Arena &Arena::operator=(Arena &&other) noexcept {
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