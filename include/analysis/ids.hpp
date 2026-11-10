#ifndef CBB_ANALYSIS_IDS_HPP
#define CBB_ANALYSIS_IDS_HPP

#include <limits>
#include <functional>

#include <cstddef>

namespace cbb {

namespace sema {

struct localid {
    std::size_t value;
    
    explicit constexpr localid(std::size_t value): value(value) {}

    static constexpr localid error() {
        return localid(std::numeric_limits<std::size_t>::max()); 
    }

    localid &operator++();

    bool operator==(const localid&) const = default;

};

} // namespace sema

    
} // namespace cbb

template<>
struct std::hash<cbb::sema::localid> {
    std::size_t operator()(const cbb::sema::localid &id) const noexcept {
        return std::hash<std::size_t>{}(id.value);
    }
};


#endif // CBB_ANALYSIS_IDS_HPP
