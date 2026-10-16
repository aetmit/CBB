#ifndef CORE_SPAN_H
#define CORE_SPAN_H

#include <stddef.h>

struct span {
    size_t offset;
    size_t length;
};

struct span span_new(size_t offset, size_t length);
size_t span_end(struct span *span);

#endif // CORE_SPAN_H