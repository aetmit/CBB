#ifndef CBB_CORE_DIAGNOSTIC_H
#define CBB_CORE_DIAGNOSTIC_H

#include "core/span.h"
#include "core/vector.h"

enum severity {
    SEV_ERROR,   
};

struct diagnostic {
    const char *msg;
    enum severity severity;
    struct span span;
    // true if msg was dynamically allocated
    // false if its just a static string literal
    bool allocated;
};

void diagnostic_destroy(struct diagnostic *diag);

GENERATE_VAL_VECTOR_DECLARATIONS(struct diagnostic, diagnostic);

#endif // CBB_CORE_DIAGNOSTIC_H