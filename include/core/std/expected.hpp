#ifndef CBB_CORE_STD_EXPECTED_HPP
#define CBB_CORE_STD_EXPECTED_HPP

namespace cbb {

namespace core {

template <class E>
class unexpected {
public:
    constexpr unexpected(const unexpected&) = default;
    constexpr unexpected(unexpected&&) = default;

    template <class Err = E>

};

template <class T, class E>
class expected {
public:
    using value_type = T;
    using error_type = E;
};

template <class E>
class expected<void, E> {

};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_STD_EXPECTED_HPP