#include "core/diagnostics.hpp"

#include "core/io.hpp"

namespace cbb {

namespace core {

diagnostic::diagnostic(std::string message, core::span span, enum severity severity): message(message), span(span), severity(severity) {}

diagnostic diagnostic::error(std::string msg, core::span span) {
    return diagnostic(msg, span, severity::error);
}

diagnostic_formatter::diagnostic_formatter(std::string_view source): source_(source) {}

void diagnostic_formatter::reset_stream() {
    ss_.str("");
    ss_.clear();
}

void diagnostic_formatter::set_source(std::string_view source) {
    source_ = source;
}

std::string diagnostic_formatter::format(const diagnostic &diagnostic) {
    reset_stream();

    source_location location = calculate_location(diagnostic.span);

    ss_ << reset << bold << "dummy/file.cbb:" << location.line << ":" << location.column << ": ";
    
    switch (diagnostic.severity) {
        case severity::error:
            ss_ << reset << bold << bright_red << "error: " << reset;
            break;
    }

    ss_ << bold << diagnostic.message << reset;

    return ss_.str();
}

diagnostic_formatter::source_location diagnostic_formatter::calculate_location(core::span span) const {
    source_location location{};

    for (std::size_t i = 0; i < span.offset(); ++i) {
        if (source_[i] == '\n') {
            ++location.line;
            location.column = 1;
        } else {
            ++location.column;
        }
    }

    // make them 1 based offsets
    ++location.column;
    ++location.line;

    return location;
}
    
} // namespace core


} // namespace cbb