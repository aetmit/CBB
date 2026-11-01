#ifndef CBB_CORE_ARENA_H
#define CBB_CORE_ARENA_H

#include <stddef.h>

struct arena_node;

struct arena {
    struct arena_node *head;
    struct arena_node *current;
};

void arena_init(struct arena *arena);
void arena_destroy(struct arena *arena);
void *arena_alloc(struct arena *arena, size_t size, size_t alignment);

#endif // CBB_CORE_ARENA_H