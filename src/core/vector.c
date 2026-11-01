#include "core/vector.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "core/io.h"

#define VECTOR_INIT_CAP 16

void vector_init(struct vector *vector, size_t elem_size)  {
    if (!vector) {
        return;
    }

    if (vector->elem_size == 0) {
        return;
    }

    if (vector->elem_size > SIZE_MAX / VECTOR_INIT_CAP) {
        return;
    }

    vector->capacity = VECTOR_INIT_CAP;
    vector->data = malloc(VECTOR_INIT_CAP * elem_size);
    if (!vector->data) {
        fatal_error("memory allocation failure");
    }
    vector->size = 0;
    vector->elem_size = elem_size;
}

void vector_destroy(struct vector *vector) {
    if (!vector) {
        return;
    }
    if (vector->data) {
        free(vector->data);
    }

    vector->data = nullptr;
}

void vector_push_back(struct vector *vector, const void *element) {
    if (!vector) {
        return;
    }

    if (vector->size >= vector->capacity) {
        if (vector->capacity > SIZE_MAX / 2) {
            fatal_error("integer overflow");
        }
        size_t new_cap = vector->capacity * 2;

        if (new_cap > SIZE_MAX / vector->elem_size) {
            fatal_error("vector element overflow");
        }

        void *new_data = realloc(vector->data, new_cap * vector->elem_size);
        if (!new_data) {
            free(vector->data);
            fatal_error("memory allocation failure");
        }
        vector->capacity = new_cap;
        vector->data = new_data;
    }

    memcpy((unsigned char *)vector->data + vector->size * vector->elem_size, element, vector->elem_size);    
    vector->size++;
}

void *vector_get(struct vector *vector, size_t index) {
    if (index >= vector->size) {
        return nullptr;
    }

    return (void *)((unsigned char *)vector->data + index * vector->elem_size);
}