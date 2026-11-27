#ifndef CBB_CORE_COMPILER_TYPE_TRAITS_HPP
#define CBB_CORE_COMPILER_TYPE_TRAITS_HPP

#include "core/compiler/config.hpp"

#if !CBB_BUILTIN_IS_CONSTRUCTIBLE
    #include "core/std/new.hpp"
    #include "core/std/utility.hpp"
    #include "core/std/__type_traits/integral_constant.hpp"
    #include "core/std/__type_traits/is_reference.hpp"
    #include "core/std/__type_traits/is_destructible.hpp"
    #include "core/std/__type_traits/enable_if.hpp"
    #include "core/std/__type_traits/declval.hpp"
#endif

namespace cbb {

namespace core {

// abstraction over compiler intristics
namespace compiler {

#if CBB_BUILTIN_IS_CONSTRUCTIBLE
    template <class T, class... Args>
    inline constexpr bool _is_constructible = __is_constructible(T, Args...);
#else

namespace detail {

template <class T, class... Args>
struct is_constructible_object {
private:
    template <class U, class... Us>
    static auto test(int) -> typename enable_if<::cbb::core::is_destructible<U>::value,
        decltype(::new (static_cast<void*>(nullptr)) U(cbb::core::declval<Us>()...), true_type{})
    >::type;

    template <class, class...>
    static false_type test(...);

public:
    using type = decltype(test<T, Args...>(0));

    static constexpr bool value = type::value;
};

template <class T, class... Args>
struct is_constructible_reference {
private:
    template <class U>
    static auto test(int) -> decltype(U(cbb::core::declval<Args>()...), true_type{});

    template <class>
    static false_type test(...);

public:
    using type = decltype(test<T>(0));

    static constexpr bool value = type::value;
};

template <class T, bool = is_reference<T>::value>
struct is_constructible_dispatch;

template <class T>
struct is_constructible_dispatch<T, false> {
    template <class... Args>
    using apply = is_constructible_object<T, Args...>;
};

template <class T>
struct is_constructible_dispatch<T, true> {
    template <class... Args>
    using apply = is_constructible_reference<T, Args...>;
};

} // namespace detail

template <class T, class... Args>
struct is_constructible : public detail::is_constructible_dispatch<T>::template apply<Args...>::type {};

template <class T, class... Args>
inline constexpr bool _is_constructible = is_constructible<T, Args...>::value;

#endif 

} // namespace compiler

} // namespace core

} // namespace cbb

#endif // CBB_CORE_COMPILER_TYPE_TRAITS_HPP