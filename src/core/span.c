#include "core/span.h"


void span_init(struct span *span, const u8 *start, size_t length) {
    if (!span) {
        return;
    }
    span->start = start;
    span->length = length;
}

void span_destroy(struct span *span) {
    if (!span) {
        return;
    }
    span->start = nullptr;
}