#ifndef CBB_LEXER_LEXER_H
#define CBB_LEXER_LEXER_H

#include <stddef.h>

#include "lexer/token.h"

struct lexer {
    // non owning view into the source buffer
    const char *source;

    size_t current;
    size_t start;
    size_t line;
    size_t column;
    size_t start_column;
    size_t source_len;
};

void lexer_init(struct lexer *lexer, const char *source);
void lexer_destroy(struct lexer *lexer);
struct token lexer_next(struct lexer *lexer);

#endif // CBB_LEXER_LEXER_H