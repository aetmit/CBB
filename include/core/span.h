#ifndef CBB_CORE_SPAN_H
#define CBB_CORE_SPAN_H

#include "core/types.h"

struct span {
    u8 *start;
    size_t length;
};

void span_init(struct span *span, u8 *start, size_t length);
void span_destroy(struct span *span);


#endif // CBB_CORE_SPAN_H