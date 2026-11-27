#ifndef CBB_CORE_STD___TYPE_TRAITS_ADD_RVALUE_REFERENCE
#define CBB_CORE_STD___TYPE_TRAITS_ADD_RVALUE_REFERENCE

#include "core/std/__type_traits/void_t.hpp"

namespace cbb {

namespace core {

template <class T, class = void>
struct add_rvalue_reference {
    using type = T;
};

template <class T>
struct add_rvalue_reference<T, void_t<T&&>> {
    using type = T&&;
};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_STD___TYPE_TRAITS_ADD_RVALUE_REFERENCE