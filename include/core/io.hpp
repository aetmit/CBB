#ifndef CBB_CORE_IO_HPP
#define CBB_CORE_IO_HPP

#include <string_view>

namespace cbb {

namespace core {

// chatgpt generated because i cannot be bothered to search each one up and write it by hand

// Text formatting
constexpr std::string_view reset      = "\033[0m";
constexpr std::string_view bold       = "\033[1m";
constexpr std::string_view dim        = "\033[2m";
constexpr std::string_view italic     = "\033[3m";
constexpr std::string_view underline  = "\033[4m";
constexpr std::string_view blink      = "\033[5m";
constexpr std::string_view reverse    = "\033[7m";
constexpr std::string_view hidden     = "\033[8m";
constexpr std::string_view strikethrough = "\033[9m";

// Foreground colours
constexpr std::string_view black   = "\033[30m";
constexpr std::string_view red     = "\033[31m";
constexpr std::string_view green   = "\033[32m";
constexpr std::string_view yellow  = "\033[33m";
constexpr std::string_view blue    = "\033[34m";
constexpr std::string_view magenta = "\033[35m";
constexpr std::string_view cyan    = "\033[36m";
constexpr std::string_view white   = "\033[37m";

// Bright foreground colours
constexpr std::string_view bright_black   = "\033[90m";
constexpr std::string_view bright_red     = "\033[91m";
constexpr std::string_view bright_green   = "\033[92m";
constexpr std::string_view bright_yellow  = "\033[93m";
constexpr std::string_view bright_blue    = "\033[94m";
constexpr std::string_view bright_magenta = "\033[95m";
constexpr std::string_view bright_cyan    = "\033[96m";
constexpr std::string_view bright_white   = "\033[97m";

// Background colours
constexpr std::string_view bg_black   = "\033[40m";
constexpr std::string_view bg_red     = "\033[41m";
constexpr std::string_view bg_green   = "\033[42m";
constexpr std::string_view bg_yellow  = "\033[43m";
constexpr std::string_view bg_blue    = "\033[44m";
constexpr std::string_view bg_magenta = "\033[45m";
constexpr std::string_view bg_cyan    = "\033[46m";
constexpr std::string_view bg_white   = "\033[47m";

} // namespace core

} // namespace cbb

#endif // CBB_CORE_IO_HPP