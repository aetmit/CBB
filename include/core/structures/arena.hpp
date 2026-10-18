#ifndef CBB_CORE_STRUCTURES_ARENA_HPP
#define CBB_CORE_STRUCTURES_ARENA_HPP

#include <memory_resource>
#include <memory>
#include <utility>
#include <vector>
#include <new>
#include <type_traits>

#include "core/types.hpp"

namespace cbb {

namespace core {

class Arena {
public:
    Arena();
    ~Arena();

    Arena(const Arena &) = delete;
    Arena &operator=(const Arena &) = delete;

    Arena(Arena &&other) noexcept;
    Arena &operator=(Arena &&other) noexcept;

    void *alloc(size_t size, size_t alignment = alignof(std::max_align_t));

    template<typename T, typename... Args>
    T *make(Args&&... args) {
        void *memory = alloc(sizeof(T), alignof(T));

        T *object = new (memory) T {
            std::forward<Args>(args)...
        };

        if constexpr (!std::is_trivially_destructible_v<T>) {
            try {
                destructors.push_back({
                    object,
                    [](void *ptr) {
                        static_cast<T *>(ptr)->~T();
                    }
                });
            } catch (...) {
                object->~T();
                throw;
            }
        }

        return object;
    }

    void reset();

private:    
    std::unique_ptr<std::pmr::monotonic_buffer_resource> resource;

    struct DestructorEntry {
        void *object;
        void (*destroy)(void *);
    };

    std::vector<DestructorEntry> destructors;
};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_STRUCTURES_ARENA_HPP