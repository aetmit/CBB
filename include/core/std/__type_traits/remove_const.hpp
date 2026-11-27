#ifndef CBB_CORE_STD___TYPE_TRAITS_REMOVE_CONST_HPP
#define CBB_CORE_STD___TYPE_TRAITS_REMOVE_CONST_HPP

namespace cbb {

namespace core {

template <class T>
struct remove_const {
    using type = T;
};

template <class T>
struct remove_const<const T> {
    using type = T;
};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_STD___TYPE_TRAITS_REMOVE_CONST_HPP
