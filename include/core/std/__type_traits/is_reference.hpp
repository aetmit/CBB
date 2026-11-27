#ifndef CBB_CORE_STD___TYPE_TRAITS_IS_REFERENCE_HPP
#define CBB_CORE_STD___TYPE_TRAITS_IS_REFERENCE_HPP

#include "core/std/__type_traits/integral_constant.hpp"

namespace cbb {

namespace core {

template <class T>
struct is_reference : public false_type {};

template <class T>
struct is_reference<T&> : public true_type {};

template <class T>
struct is_reference<T&&> : public true_type {};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_STD___TYPE_TRAITS_IS_REFERENCE_HPP