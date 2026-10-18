#include "sema/hir.hpp"

namespace cbb {

namespace sema {

Literal::Literal(u32 i): as{ .integer = i } {}

} // namespace sema

} // namespace cbb