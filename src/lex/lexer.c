#include "lex/lexer.h"

#include <string.h>
#include <stddef.h>

void lexer_init(struct lexer *lexer, const char *source) {
    lexer->source = source;
    memset(lexer + offsetof(struct lexer, current), (unsigned char)0, offsetof(struct lexer, line) + sizeof(size_t) - offsetof(struct lexer, current));
}

void lexer_destroy(struct lexer *lexer) {
    lexer->source = NULL;
    memset(lexer + offsetof(struct lexer, current), (unsigned char)0, offsetof(struct lexer, line) + sizeof(size_t) - offsetof(struct lexer, current));
}

struct token lexer_next(struct lexer *lexer) {

}