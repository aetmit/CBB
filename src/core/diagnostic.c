#include "core/diagnostics.h"

#include <stdlib.h>

GENERATE_VAL_VECTOR_IMPLEMENTATION(struct diagnostic, diagnostic);

void diagnostic_destroy(struct diagnostic *diag) {
    if (diag->allocated) {
        free(diag->msg); 
    }
    diag->msg = nullptr;
}