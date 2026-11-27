#ifndef CBB_CORE_STD_CSTDDEF_HPP
#define CBB_CORE_STD_CSTDDEF_HPP

#include <stddef.h>

namespace cbb {

namespace core {

using size_t = ::size_t;
using ptrdiff_t = ::ptrdiff_t;
using max_align_t = ::max_align_t;

typedef decltype(nullptr) nullptr_t;

} // namespace core

} // namespace cbb

#endif // CBB_CORE_STD_CSTDDEF_HPP