#include "lexer/lexer.h"

#include <ctype.h>
#include <string.h>

#include "core/str.h"
#include "core/io.h"

void lexer_init(struct lexer *lexer, const char *source) {
    lexer->source = source;

    lexer->column = 0;
    lexer->current = 0;
    lexer->line = 0;
    lexer->start = 0;
    lexer->start_column = 0;

    lexer->source_len = strlen(source);
}

void lexer_destroy(struct lexer *lexer) {
    lexer->source = nullptr;
}

static bool is_at_end(struct lexer *lexer) {
    return lexer->current >= lexer->source_len; 
}

static struct token create_eof(struct lexer *lexer) {
    struct span span;
    span_init(&span, lexer->source + lexer->start, lexer->current - lexer->start);
    return (struct token){ .type = TK_EOF, .span = span, .column = lexer->column, .line = lexer->line };
}

static char peek(struct lexer *lexer) {
    // we don't need to check for eof here because on eof it will just return \0 since the source string is null terminated
    return lexer->source[lexer->current];
}

static char advance(struct lexer *lexer) {
    char c = peek(lexer);

    if (is_at_end(lexer)) {
        // return the null byte
        return c;
    }

    if (c == '\n') {
        lexer->line++;
        lexer->column = 0;
    } else {
        lexer->column++;
    }

    lexer->current++;

    return c;
}

static void skip_whitespace(struct lexer *lexer) {
    while (!is_at_end(lexer) && isspace((unsigned char)peek(lexer))) {
        advance(lexer);
    }
}

static struct token new_token(struct lexer *lexer, enum tokentype type) {
    struct span span;
    span_init(&span, lexer->source + lexer->start, lexer->current - lexer->start);
    return (struct token){ .type = type, .span = span, .column = lexer->start_column, .line = lexer->line };
}

struct keyword_entry {
    const char *lexeme;
    enum tokentype value;
};

static const struct keyword_entry keywords[] = {
    { .lexeme = "int", .value = TK_INT },
};

static const size_t keyword_count = sizeof(keywords) / sizeof(keywords[0]);

static enum tokentype get_keyword(const char *s) {
    for (size_t i = 0; i < keyword_count; ++i) {
        if (strcmp(s, keywords[i].lexeme) == 0) {
            return keywords[i].value;
        }
    }
    return TK_IDENTIFIER;
}

struct token lexer_next(struct lexer *lexer) {
    skip_whitespace(lexer);

    if (is_at_end(lexer)) {
        return create_eof(lexer); 
    }

    lexer->start = lexer->current;
    lexer->start_column = lexer->column;

    char c = peek(lexer);

    switch (c) {
        case '(':
            advance(lexer);
            return new_token(lexer, TK_OPENPAREN);
        case ')':
            advance(lexer);
            return new_token(lexer, TK_CLOSEPAREN);
        case '{':
            advance(lexer);
            return new_token(lexer, TK_OPENBRACE);
        case '}':
            advance(lexer);
            return new_token(lexer, TK_CLOSEBRACE);
        case '=':
            advance(lexer);
            return new_token(lexer, TK_EQUAL);
        case '+':
            advance(lexer);
            return new_token(lexer, TK_PLUS);
        case '-':
            advance(lexer);
            return new_token(lexer, TK_MINUS);
        case '*':
            advance(lexer);
            return new_token(lexer, TK_STAR);
        case '/':
            advance(lexer);
            return new_token(lexer, TK_SLASH);
        case ';':
            advance(lexer);
            return new_token(lexer, TK_SEMICOLON);
        default:
            if (isdigit((unsigned char)c)) {
                while (!is_at_end(lexer) && isdigit((unsigned char)peek(lexer))) {
                    advance(lexer);
                }
                return new_token(lexer, TK_INTEGER_LITERAL);
            } else if (isalpha((unsigned char)c) || c == '_') {
                struct dyn_string str;
                dyn_string_init(&str);

                while (!is_at_end(lexer) && (isalnum((unsigned char)peek(lexer)) || peek(lexer) == '_')) {
                    dyn_string_push_back(&str, peek(lexer));
                    advance(lexer);
                }

                char *lexeme = dyn_string_c_str(&str);

                enum tokentype type = get_keyword(lexeme);

                dyn_string_destroy(&str);

                return new_token(lexer, type);
            } else {
                // to be fixed in the future
                fatal_error("currently abort when unknown character is encountered");
            }
    }
}
