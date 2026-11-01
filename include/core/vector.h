#ifndef CBB_CORE_VECTOR_H
#define CBB_CORE_VECTOR_H

#include <stddef.h>

struct vector {
    void *data;
    size_t size;
    size_t capacity;
    size_t elem_size;
};

void vector_init(struct vector *vector, size_t elem_size);
void vector_destroy(struct vector *vector);
void vector_push_back(struct vector *vector, const void *element);
void *vector_get(struct vector *vector, size_t index);

#endif // CBB_CORE_VECTOR_H