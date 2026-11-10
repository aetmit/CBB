#ifndef CBB_CORE_CONCEPTS_HPP
#define CBB_CORE_CONCEPTS_HPP

#include <concepts>
#include <ostream>

namespace cbb {

namespace core {

template <class T>
concept writable_to_stream = requires(std::ostream &os, const T &val) {
    os << val;
};

} // namespace core

} // namespace cbb


#endif // CBB_CORE_CONCEPTS_HPP