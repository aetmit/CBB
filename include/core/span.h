#ifndef CBB_CORE_SPAN_H
#define CBB_CORE_SPAN_H

#include "core/types.h"

struct span {
    const u8 *start;
    size_t length;
};

void span_init(struct span *span, const u8 *start, size_t length);
void span_destroy(struct span *span);


#endif // CBB_CORE_SPAN_H