#include "core/str.h"

#include <stdlib.h>

#include "core/io.h"

#define DYN_STRING_INIT_CAP 16

void dyn_string_init(struct dyn_string *str) {
    if (!str) {
        return;
    }
    str->capacity_ = DYN_STRING_INIT_CAP;
    str->size_ = 0;
    str->data_ = malloc(sizeof(char) * str->capacity_);
    if (!str->data_) {
        // no return
        fatal_error("memory allocation failure");
    }
    str->data_[str->size_] = '\0';
}

void dyn_string_destroy(struct dyn_string *str) {
    if (!str) {
        return;
    }
    str->size_ = 0;
    str->capacity_ = 0;
    if (str->data_) {
        free(str->data_);
    }
    str->data_ = nullptr;
}

static void reallocate(struct dyn_string *str, size_t new_cap) {
    char *tmp = realloc(str->data_, new_cap);
    if (!tmp) {
        // no return
        fatal_error("memory allocation failure");
    }

    str->data_ = tmp;
    str->capacity_ = new_cap;
}

void dyn_string_push_back(struct dyn_string *str, char c) {
    if (!str) {
        return;
    }
    if (str->size_ + 1 >= str->capacity_) {
        reallocate(str, str->capacity_ * 2);
    }

    str->data_[str->size_++] = c;
    str->data_[str->size_] = '\0';
}

char *dyn_string_c_str(struct dyn_string *str) {
    if (!str) {
        return nullptr;
    }
    return str->data_;
}
