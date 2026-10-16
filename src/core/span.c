#include "core/span.h"

struct span span_new(size_t offset, size_t length) {
    return (struct span){ .length = length, .offset = offset };
}

size_t span_end(struct span *span) {
    return span->offset + span->length;
}