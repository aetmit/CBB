#ifndef CBB_CORE_ERROR_HPP
#define CBB_CORE_ERROR_HPP

namespace cbb {

namespace core {

void fatal_error(const char *msg);

#define UNREACHABLE() \
    char buff[128]; \
    snprintf(buff, sizeof(buff), "entered unreachable code at %s:%d", __FILE__, __LINE__); \
    core::fatal_error(buff);

} // namespace core

} // namespace cbb

#endif // CBB_CORE_ERROR_HPP