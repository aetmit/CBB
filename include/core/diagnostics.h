#ifndef CBB_CORE_DIAGNOSTIC_H
#define CBB_CORE_DIAGNOSTIC_H

#include "core/span.h"

enum severity {
    SEV_ERROR,   
};

struct diagnostic {
    const char *msg;
    enum severity severity;
    struct span span;
};

#endif // CBB_CORE_DIAGNOSTIC_H