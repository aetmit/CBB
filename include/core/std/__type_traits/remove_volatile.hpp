#ifndef CBB_CORE_STD___TYPE_TRAITS_REMOVE_VOLATILE_HPP
#define CBB_CORE_STD___TYPE_TRAITS_REMOVE_VOLATILE_HPP

namespace cbb {

namespace core {

template <class T>
struct remove_volatile {
    using type = T;
};

template <class T>
struct remove_volatile<volatile T> {
    using type = T;
};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_STD___TYPE_TRAITS_REMOVE_VOLATILE_HPP