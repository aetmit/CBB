#ifndef CBB_CORE_STD___TYPE_TRAITS_REMOVE_REFERENCE_HPP
#define CBB_CORE_STD___TYPE_TRAITS_REMOVE_REFERENCE_HPP

namespace cbb {

namespace core {

template <class T>
struct remove_reference {
    using type = T;
};

template <class T>
struct remove_reference<T&> {
    using type = T;
};

template <class T>
struct remove_reference<T&&> {
    using type = T;
};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_STD___TYPE_TRAITS_REMOVE_REFERENCE_HPP