#ifndef CBB_CORE_STD___TYPE_TRAITS_ENABLE_IF_HPP
#define CBB_CORE_STD___TYPE_TRAITS_ENABLE_IF_HPP

namespace cbb {

namespace core {

template <bool B, class T = void>
struct enable_if {};

template <class T>
struct enable_if<true, T> {
    using type = T;
};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_STD___TYPE_TRAITS_ENABLE_IF_HPP