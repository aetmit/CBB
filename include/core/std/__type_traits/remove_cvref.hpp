#ifndef CBB_CORE_STD___TYPE_TRAITS_REMOVE_CVREF_HPP
#define CBB_CORE_STD___TYPE_TRAITS_REMOVE_CVREF_HPP

#include "core/std/__type_traits/remove_cv.hpp"
#include "core/std/__type_traits/remove_reference.hpp"

namespace cbb {

namespace core {

template <class T>
struct remove_cvref {
    using type = typename remove_cv<typename remove_reference<T>::type>::type;
};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_STD___TYPE_TRAITS_REMOVE_CVREF_HPP
