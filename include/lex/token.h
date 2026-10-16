#ifndef LEX_TOKEN_H
#define LEX_TOKEN_H

#include <stddef.h>

#include "core/span.h"

enum tokentype {
    TK_INTERGER_LITERAL,

    TK_OPENPAREN,
    TK_CLOSEPAREN,

    TK_PLUS,
    TK_MINUS,
    TK_STAR,
    TK_SLASH,

    TK_EOF,
};

struct token {
    enum tokentype type;
    struct span span;
    size_t line;
    size_t column;
};

#endif // LEX_TOE