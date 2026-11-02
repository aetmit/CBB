#ifndef CBB_CORE_VECTOR_H
#define CBB_CORE_VECTOR_H

#include <stddef.h>
#include <assert.h>

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

#define GENERATE_PTR_VECTOR_DECLARATIONS(type, name)                                    \
struct name##_vector {                                                                  \
    struct vector inner;                                                                \
};                                                                                      \
void name##_vector_init(struct name##_vector *vector);                                  \
void name##_vector_destroy(struct name##_vector *vector);                               \
void name##_vector_push_back(struct name##_vector *vector, const type *element);        \
type *name##_vector_get(struct name##_vector *vector, size_t index);                    \

#define GENERATE_VAL_VECTOR_DECLARATIONS(type, name)                                    \
struct name##_vector {                                                                  \
    struct vector inner;                                                                \
};                                                                                      \
void name##_vector_init(struct name##_vector *vector);                                  \
void name##_vector_destroy(struct name##_vector *vector);                               \
void name##_vector_push_back(struct name##_vector *vector, const type element);         \
type name##_vector_get(struct name##_vector *vector, size_t index);                     \

#define GENERATE_PTR_VECTOR_IMPLEMENTATION(type, name)                                  \
void name##_vector_init(struct name##_vector *vector) {                                 \
    vector_init(&vector->inner, sizeof(type));                                          \
}                                                                                       \
                                                                                        \
void name##_vector_destroy(struct name##_vector *vector) {                              \
    vector_destroy(&vector->inner);                                                     \
}                                                                                       \
                                                                                        \
void name##_vector_push_back(struct name##_vector *vector, const type *element) {       \
    vector_push_back(&vector->inner, element);                                          \
}                                                                                       \
                                                                                        \
type *name##_vector_get(struct name##_vector *vector, size_t index) {                   \
    if (index >= vector->inner.size) {                                                  \
        return nullptr;                                                                 \
    }                                                                                   \
    return (type *)vector_get(&vector->inner, index);                                   \
}                                                                                       \

#define GENERATE_VAL_VECTOR_IMPLEMENTATION(type, name)                                  \
void name##_vector_init(struct name##_vector *vector) {                                 \
    vector_init(&vector->inner, sizeof(type));                                          \
}                                                                                       \
                                                                                        \
void name##_vector_destroy(struct name##_vector *vector) {                              \
    vector_destroy(&vector->inner);                                                     \
}                                                                                       \
                                                                                        \
void name##_vector_push_back(struct name##_vector *vector, const type element) {        \
    vector_push_back(&vector->inner, &element);                                         \
}                                                                                       \
                                                                                        \
type name##_vector_get(struct name##_vector *vector, size_t index) {                    \
    assert(index < vector->inner.size);                                                 \
    return *((type *)vector_get(&vector->inner, index));                                \
}                                                                                       \

#endif // CBB_CORE_VECTOR_H