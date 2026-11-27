#ifndef CBB_CORE_STD___TYPE_TRAITS_HPP_IS_DESTRUCTIBLE
#define CBB_CORE_STD___TYPE_TRAITS_HPP_IS_DESTRUCTIBLE

#include "core/std/__type_traits/integral_constant.hpp"
#include "core/std/__type_traits/void_t.hpp"
#include "core/std/__type_traits/remove_all_extents.hpp"

namespace cbb {

namespace core {

namespace detail {

template <class T, class = void>
struct is_destructible_impl : public false_type {};

template <class T>
struct is_destructible_impl<T, void_t<decltype(declval<T&>().~T())>> : public true_type {};

} // namespace detail

template <typename T>
struct is_destructible : public detail::is_destructible_impl<typename remove_all_extents<T>::type> {};

template <>
struct is_destructible<void> : public false_type {};

template <>
struct is_destructible<const void> : public false_type {};

template <>
struct is_destructible<volatile void> : public false_type {};

template <>
struct is_destructible<const volatile void> : public false_type {};

template <class T>
struct is_destructible<T[]> : public false_type {};

template <class T>
struct is_destructible<T&> : public true_type {};

template <class T>
struct is_destructible<T&&> : public true_type {};

    
} // namespace core

} // namespace cbb


#endif // CBB_CORE_STD___TYPE_TRAITS_HPP_IS_DESTRUCTIBLE