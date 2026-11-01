#include "core/arena.h"

#include <stdlib.h>
#include <stdint.h>

#include "core/io.h"

#define INIT_BLOCK_SIZE (8 * 1024)

struct arena_node {
    struct arena_node *next;
    void *data;
    size_t capacity;
    size_t offset;
};

static struct arena_node *alloc_node(size_t capacity) {
    struct arena_node *node = malloc(sizeof(struct arena_node));
    if (!node) {
        fatal_error("memory allocation failure");
    }
    node->next = nullptr;
    node->capacity = capacity;
    node->data = malloc(node->capacity);

    if (!node->data) {
        fatal_error("memory allocation failure");
    }
    node->offset = 0;

    return node;
}

void arena_init(struct arena *arena) {
    if (!arena) {
        return;
    }
    struct arena_node *node = alloc_node(INIT_BLOCK_SIZE);

    arena->current = node;
    arena->head = node;
}

void arena_destroy(struct arena *arena) {
    if (!arena) {
        return;
    }
    struct arena_node *node = arena->head;
    while (node) {
        free(node->data);
        struct arena_node *next = node->next;
        free(node);
        node = next;
    }
    arena->head = nullptr;
    arena->current = nullptr;
}

// i got no clue how this works but its works so yea
static uintptr_t align_up(uintptr_t address, size_t alignment) {
    return (address + alignment - 1) & ~(uintptr_t)(alignment - 1);
}

void *arena_alloc(struct arena *arena, size_t size, size_t alignment) {
    if (!arena) {
        return nullptr;
    }

    if (size == 0) {
        size = 1;
    }

    // check if alignment is a power of 2
    if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
        return nullptr;
    }

    struct arena_node *node = arena->current;

    uintptr_t address = (uintptr_t)node->data + node->offset;
    uintptr_t aligned = align_up(address, alignment);

    size_t padding = aligned - address;

    if (padding <= node->capacity - node->offset && size <= node->capacity - node->offset - padding) {
        node->offset += padding + size;
        return (void *)aligned;
    }

    if (size > SIZE_MAX - (alignment - 1))  {
        return nullptr;
    }

    size_t capacity = size + alignment - 1;

    if (capacity < INIT_BLOCK_SIZE) {
        capacity = INIT_BLOCK_SIZE;
    }

    struct arena_node *new_node = alloc_node(capacity);

    node->next = new_node;
    arena->current = new_node;

    address = (uintptr_t)new_node->data;
    aligned = align_up(address, alignment);

    padding = aligned - address;
    new_node->offset = padding + size;

    return (void *)aligned;
}