#ifndef CBB_CORE_STD___TYPE_TRAITS_HPP
#define CBB_CORE_STD___TYPE_TRAITS_HPP

#include "core/std/cstddef.hpp"

namespace cbb {

namespace core {

template <class T>
struct remove_all_extents {
    using type = T;
};

template <class T>
struct remove_all_extents<T[]> {
    using type = typename remove_all_extents<T>::type;
};

template <class T, size_t N>
struct remove_all_extents<T[N]> {
    using type = typename remove_all_extents<T>::type;
};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_STD___TYPE_TRAITS_HPP