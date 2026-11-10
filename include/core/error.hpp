#ifndef CBB_CORE_ERROR_HPP
#define CBB_CORE_ERROR_HPP

#include <iostream>
#include <format>
#include <string_view>

#include <cstdlib>

namespace cbb {

namespace core {

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define LOCATION __FILE__ ":" TOSTRING(__LINE__)

template <class ...Args>
[[noreturn]] void fatal_error(std::string_view location, std::format_string<Args...> fmt, Args&&... args) {
    std::cerr << "fatal error: ";
    std::cerr << location << ": ";
    std::cerr << std::format(fmt, std::forward<Args>(args)...) << '\n';

    std::exit(EXIT_FAILURE);
}

#define FATAL(...) ::cbb::core::fatal_error(LOCATION, __VA_ARGS__)

#define UNREACHABLE() FATAL("entered unreachable code")

#define UNIMPLEMENTED() FATAL("entered unimplemented code");

} // namespace core

} // namespace cbb


#endif // CBB_CORE_ERROR_HPP