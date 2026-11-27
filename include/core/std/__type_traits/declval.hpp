#ifndef CBB_CORE_STD___TYPE_TRAITS_DECLVAL_HPP
#define CBB_CORE_STD___TYPE_TRAITS_DECLVAL_HPP

#include "core/std/__type_traits/add_rvalue_reference.hpp"

namespace cbb {

namespace core {

template <class T>
typename add_rvalue_reference<T>::type declval() noexcept;
    
} // namespace core
    
} // namespace cbb


#endif // CBB_CORE_STD___TYPES_TRAITS_DECLVAL_HPP