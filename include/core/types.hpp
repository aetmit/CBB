#ifndef CORE_TYPES_HPP
#define CORE_TYPES_HPP

#include <cstdint>
#include <cstddef>


// these do not go into core:: because using core::u8 everytime we wanna use u8 is annoying
namespace cbb {

// do this because i dont wanna write std::size_t cuz its annoying
using std::size_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

} // namespace cbb

#endif // CORE_TYPES_HPP