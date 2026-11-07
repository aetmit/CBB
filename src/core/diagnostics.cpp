#include "core/diagnostics.hpp"

namespace cbb {

namespace core {

diagnostic::diagnostic(std::string message, core::span span, enum severity severity): message(message), span(span), severity(severity) {}

diagnostic diagnostic::error(std::string msg, core::span span) {
    return diagnostic(msg, span, severity::error);
}
    
} // namespace core


} // namespace cbb