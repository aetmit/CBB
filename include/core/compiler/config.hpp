#ifndef CBB_CORE_COMPILER_CONFIG_HPP
#define CBB_CORE_COMPILER_CONFIG_HPP

namespace cbb {

namespace core {

#ifndef __has_builtin
    #define __has_builtin(x) 0
#endif

#if __has_builtin(__is_constructible) || defined(_MSC_VER)
    #define CBB_BUILTIN_IS_CONSTRUCTIBLE 1
#else
    #define CBB_BUILTIN_IS_CONSTRUCTIBLE 0
#endif

#if __has_builtin(__is_trivially_constructible) || defined(_MSC_VER)
    #define CBB_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE 1
#else
    #define CBB_BUILTIN_IS_TRIVIALLY_CONSTRUCTIBLE 0
#endif

#if __has_builtin(__is_nothrow_constructible) || defined(_MSC_VER)
    #define CBB_BUILTIN_IS_NOTHROW_CONSTRUCTIBLE 1
#else
    #define CBB_BUILTIN_IS_NOTHROW_CONSTRUCTIBLE 0
#endif


} // namespace core

} // namespace cbb

#endif // CBB_CORE_COMPILER_CONFIG_HPP