#ifndef CBB_CORE_DIAGNOSTICS_HPP
#define CBB_CORE_DIAGNOSTICS_HPP

#include <string>
#include <string_view>
#include <sstream>

#include <cstddef>

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

class diagnostic_formatter {
public:
    diagnostic_formatter(std::string_view source);

    std::string format(const diagnostic &diagnostic);

    void set_source(std::string_view source);
private:
    std::stringstream ss_;
    void reset_stream();

    std::string_view source_;

    struct source_location {
        std::size_t line;
        std::size_t column;
    };

    source_location calculate_location(core::span span) const;
};

} // namespace core

} // namespace cbb

#endif // CBB_CORE_DIAGNOSTICS_HPP