#ifndef CBB_CORE_STD___TYPE_TRAITS_IS_SAME_HPP
#define CBB_CORE_STD___TYPE_TRAITS_IS_SAME_HPP

#include "core/std/__type_traits/integral_constant.hpp"

namespace cbb {

namespace core {

template <class T, class U>
struct is_same : public false_type {};

template <class T>
struct is_same<T, T> : public true_type {};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_STD___TYPE_TRAITS_IS_SAME_HPP