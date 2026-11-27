#ifndef CBB_CORE_STD_UTILITY_HPP
#define CBB_CORE_STD_UTILITY_HPP

#include "core/std/type_traits.hpp"

namespace cbb {

namespace core {

struct in_place_t {
    explicit in_place_t() = default;
};

inline constexpr in_place_t in_place{};

template <class T>
constexpr T&& forward(typename remove_reference<T>::type& t) noexcept {
    return static_cast<T&&>(t);
}

template <class T, class = typename enable_if<!is_lvalue_reference<T>::value>::type>
constexpr T&& forward(typename remove_reference<T>::type &&t) noexcept {
    return static_cast<T&&>(t);
}
    
} // namespace core

} // namespace cbb

#endif // CBB_CORE_STD_UTILITY_HPP