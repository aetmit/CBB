#ifndef CBB_ANALYSIS_IDS_HPP
#define CBB_ANALYSIS_IDS_HPP

#include <limits>
#include <functional>
#include <ostream>

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

std::ostream &operator<<(std::ostream &os, const localid &id);

struct hirid {
    std::size_t value;

    explicit constexpr hirid(std::size_t value): value(value) {}

    bool operator==(const hirid&) const = default;
};

std::ostream &operator<<(std::ostream &os, const hirid &hirid);

} // namespace sema

} // namespace cbb

template <>
struct std::hash<cbb::sema::localid> {
    std::size_t operator()(const cbb::sema::localid &id) const noexcept {
        return std::hash<std::size_t>{}(id.value);
    }
};


#endif // CBB_ANALYSIS_IDS_HPP
