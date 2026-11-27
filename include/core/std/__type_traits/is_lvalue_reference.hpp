#ifndef CBB_CORE_STD___TYPE_TRAITS_IS_LVALUE_REFERENCE_HPP
#define CBB_CORE_STD___TYPE_TRAITS_IS_LVALUE_REFERENCE_HPP

namespace cbb {

namespace core {

template <class T>
struct is_lvalue_reference : public false_type {};

template <class T>
struct is_lvalue_reference<T&> : public true_type {};
    
} // namespace core

} // namespace cbb


#endif // CBB_CORE_STD___TYPE_TRAITS_IS_LVALUE_REFERENCE_HPP