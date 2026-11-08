#ifndef CBB_ANALYSIS_IDS_HPP
#define CBB_ANALYSIS_IDS_HPP

#include <limits>

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

};

} // namespace sema

    
} // namespace cbb


#endif // CBB_ANALYSIS_IDS_HPP
