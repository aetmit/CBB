#ifndef CBB_PARSER_PARSER_H
#define CBB_PARSER_PARSER_H

#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/ast.h"

struct parser {
    struct lexer tokenstream;

    const char *source;

    struct token current;

    struct nodeid current_nodeid;
};

#endif // CBB_PARSER_PARSER_H