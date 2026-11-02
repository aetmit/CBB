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

template<typename ...Args>
[[noreturn]] void fatalError(std::string_view location, std::format_string<Args...> fmt, Args&&... args) {
    std::cerr << location;
    std::cerr << std::format(fmt, std::forward<Args>(args)...) << '\n';

    std::exit(EXIT_FAILURE);
}

#define FATAL(...) fatalError(LOCATION, __VA_ARGS__)

} // namespace core

} // namespace cbb


#endif // CBB_CORE_ERROR_HPP