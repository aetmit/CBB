#ifndef CBB_CORE_STD_NEW_HPP
#define CBB_CORE_STD_NEW_HPP

#include "core/std/cstddef.hpp"

// NOTE: because libc++ already defined operator new and delete for us
// we cannot implement these outselves unless we mess with include flags
// and i cant be bothered

#include <new>

namespace cbb {

namespace core {

enum class align_val_t : ::cbb::core::size_t {};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_STD_NEW_HPP