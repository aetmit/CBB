#include "core/error.hpp"

#include <iostream>
#include <cstdlib>

namespace cbb {

namespace core {

void fatal_error(const char *msg) {
    std::cerr << "fatal: " << msg << "\n";
    std::exit(EXIT_FAILURE);
}

} // namespace core

} // namespace cbb