#ifndef CBB_CORE_UTILITY_HPP
#define CBB_CORE_UTILITY_HPP

#include <concepts>

#include "core/concepts.hpp"

namespace cbb {

namespace core {

template <class T, class... Patterns>
constexpr bool matches(const T& value, Patterns&&... patterns) {
    auto check = [&](const auto &pattern) {
        if constexpr (std::invocable<decltype(pattern)&, const T&>) {
            return pattern(value);
        } else {
            static_assert(core::equlity_comparable<T, decltype(pattern)>, "Pattern must be invocable with value or support operator==");

            return value == pattern;
        }
    };

    return (check(patterns) || ...);
}

} // namespace core

} // namespace cbb

#endif // CBB_CORE_UTILITY_HPP