#include "core/span.h"


void span_init(struct span *span, u8 *start, size_t length) {
    span->start = start;
    span->length = length;
}

void span_destroy(struct span *span) {
    span->start = nullptr;
}