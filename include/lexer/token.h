#ifndef CBB_LEXER_TOKEN_H
#define CBB_LEXER_TOKEN_H

#include <stddef.h>

#include "core/span.h"

enum tokentype {
    TK_INTEGER_LITERAL,
    TK_IDENTIFIER,
    
    TK_OPENPAREN,
    TK_CLOSEPAREN,

    TK_OPENBRACE,
    TK_CLOSEBRACE,

    TK_EQUAL,

    // not to be confused with TK_INTEGER_LITERAL
    // TK_INT is the actual int keyword
    TK_INT,

    TK_PLUS,
    TK_MINUS,
    TK_STAR,
    TK_SLASH,

    TK_SEMICOLON,

    TK_EOF,
};

struct token {
    enum tokentype type;
    struct span span;
    size_t column;
    size_t line;
};

#endif // CBB_LEXER_TOKEN_H