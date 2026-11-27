#ifndef CBB_CORE_STD___TYPE_TRAITS_REMOVE_CV_HPP
#define CBB_CORE_STD___TYPE_TRAITS_REMOVE_CV_HPP

#include "core/std/__type_traits/remove_const.hpp"
#include "core/std/__type_traits/remove_volatile.hpp"

namespace cbb {

namespace core {

template <class T>
struct remove_cv {
    using type = typename remove_const<typename remove_volatile<T>::type>::type;
};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_STD___TYPE_TRAITS_REMOVE_CV_HPP