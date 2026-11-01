#ifndef CBB_PARSER_AST_H
#define CBB_PARSER_AST_H

#include <stddef.h>

#include "core/span.h"

struct nodeid {
    size_t value;
};

#define NODEID(id) (struct nodeid){ .value = id }

struct exprtype {

};

struct expr {
    struct nodeid id;
    struct span span;
    struct exprtype type;
};

#endif // CBB_PARSER_AST_H