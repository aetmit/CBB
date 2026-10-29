#ifndef CBB_CORE_STR_H
#define CBB_CORE_STR_H

#include <stddef.h>

struct dyn_string {
    char *data_;
    size_t size_;
    size_t capacity_;
};

void dyn_string_init(struct dyn_string *str);
void dyn_string_destroy(struct dyn_string *str);
void dyn_string_push_back(struct dyn_string *str, char c);
char *dyn_string_c_str(struct dyn_string *str);

#endif // CBB_CORE_STR_H