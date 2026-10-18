#ifndef CBB_CORE_DIAGNOSTICS_HPP
#define CBB_CORE_DIAGNOSTICS_HPP

#include "core/span.hpp"

namespace cbb {

namespace core {

enum class Severity {
    Error,
};

struct Diagnostic {
    Severity severity;
    const char *message;
    Span span;
    // true if message was dynamically allocated
    // false if not
    bool allocated;

    Diagnostic(Severity severity, const char *message, bool isMsgDynamicallyAllocated, Span span);
    ~Diagnostic();
};

} // namespace core

} // namespace cbb


#endif // CBB_CORE_DIAGNOSTICS_HPP
