#ifndef CBB_CORE_STD___TYPE_TRAITS_IS_CONSTRUCTIBLE_HPP
#define CBB_CORE_STD___TYPE_TRAITS_IS_CONSTRUCTIBLE_HPP

#include "core/compiler/type_traits.hpp"

namespace cbb {

namespace core {

template <class T, class... Args>
struct is_constructible : public bool_constant<compiler::_is_constructible<T, Args...>> {};

} // namespace core
    
} // namespace cbb


#endif // CBB_CORE_STD___TYPE_TRAITS_IS_CONSTRUCTIBLE_HPP