#include "core/diagnostics.hpp"

namespace cbb {

namespace core {

Diagnostic::Diagnostic(Severity severity, const char *message, bool isMsgDynamicallyAllocated, Span span):
    severity(severity), message(message), span(span), allocated(isMsgDynamicallyAllocated) {}

Diagnostic::~Diagnostic() {
    if (allocated) {
        delete[] message;
    }
    message = nullptr;
}

} // namespace core

} // namespace cbb