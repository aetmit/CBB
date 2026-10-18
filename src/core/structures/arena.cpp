#include "core/structures/arena.hpp"

namespace cbb {

namespace core {

void *Arena::alloc(size_t size, size_t alignment) {
    return resource.allocate(size, alignment);
}

Arena::~Arena() {
    reset();
}

void Arena::reset() {
    for (auto it = destructors.rbegin(); it != destructors.rend(); ++it) {
        it->destroy(it->object);
    }

    destructors.clear();
    resource.release();
}

} // namespace core 

} // namespace cbb