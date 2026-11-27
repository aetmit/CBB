#ifndef CBB_CORE_STD___TYPE_TRAITS_INTEGRAL_CONSTANT_HPP
#define CBB_CORE_STD___TYPE_TRAITS_INTEGRAL_CONSTANT_HPP

namespace cbb {

namespace core {

template <class T, T v>
struct integral_constant {
    using value_type = T;
    using type = integral_constant;

    static constexpr T value = v;

    constexpr operator value_type() const noexcept {
        return value; 
    }

    constexpr value_type operator()() const noexcept {
        return value;
    }
};

template <bool B>
using bool_constant = integral_constant<bool, B>;

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

} // namespace core

} // namespace cbb

#endif // CBB_CORE_STD___TYPE_TRAITS_INTEGRAL_CONSTANT_HPP