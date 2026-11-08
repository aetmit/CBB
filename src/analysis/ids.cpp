#include "analysis/ids.hpp"

namespace cbb {

namespace sema {

localid &localid::operator++() {
    value++;
    return *this;
}
    
} // namespace sema

    
} // namespace cbb
