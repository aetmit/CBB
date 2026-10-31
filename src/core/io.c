#include "core/io.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

[[noreturn]] void fatal_error(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);

    fprintf(stderr, "fatal: %s:%d", __FILE__, __LINE__);
    
    vfprintf(stderr, fmt, args);

    fprintf(stderr, "\n");

    va_end(args);

    exit(1);
}