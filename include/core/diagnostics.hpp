#ifndef CBB_CORE_DIAGNOSTICS_HPP
#define CBB_CORE_DIAGNOSTICS_HPP

#include <string>

#include "core/span.hpp"

namespace cbb {

namespace core {

enum class severity {
    error,   
};

struct diagnostic {
    std::string message;
    core::span span;
    severity severity;

    diagnostic(std::string message, core::span span, enum severity severity);

    static diagnostic error(std::string message, core::span span);
};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_DIAGNOSTICS_HPP