#ifndef CBB_ANALYSIS_TYPE_HPP
#define CBB_ANALYSIS_TYPE_HPP

#include <ostream>

namespace cbb {

namespace sema {

enum class type {
    int_,

    bool_,

    error,
};

std::ostream &operator<<(std::ostream &os, const type &type);

enum class value_cat {
    lvalue,
    rvalue,

    invalid,
};

struct type_info {
    type type; 
    value_cat category;

    constexpr type_info(enum type type, value_cat category): type(type), category(category) {}

    static const type_info error;

    bool operator==(const type_info &other) const;
    bool operator!=(const type_info &other) const;
};

inline constexpr type_info type_info::error(type::error, value_cat::invalid);
    
} // namespace sema

    
} // namespace cbb


#endif // CBB_ANALYSIS_TYPE_HPP