#ifndef LEX_LEXER_H
#define LEX_LEXER_H

#include <stddef.h>

#include "lex/token.h"

struct lexer {
    // non owning 
    // pointer into source string
    const char *source; 

    // NOTE: do not insert any new fields between these 5 fields because then it will 
    // break memset in the initialisation function lexer_init(...) as well as the destructor function 
    // lexer_destroy(...)
    size_t current;
    size_t start;
    size_t column;
    size_t start_column;
    size_t line;
};

void lexer_init(struct lexer *lexer, const char *source);
void lexer_destroy(struct lexer *lexer);
struct token lexer_next(struct lexer *lexer);

#endif // LEX_LEXER_H
