#ifndef CBB_PARSER_AST_H
#define CBB_PARSER_AST_H

#include <stddef.h>

#include "core/span.h"
#include "core/arena.h"
#include "core/types.h"
#include "analysis/types.h"

struct nodeid {
    size_t value;
};

#define NODEID(id) (struct nodeid){ .value = id }

struct expr;
struct stmt;

enum exprtag {
    EX_LITERAL,
    EX_BINARY,
    EX_UNARY,
    EX_VARIABLE,
    EX_VARASSIGN,

    EX_ERROR,
};

enum literaltag {
    LIT_INTEGER,
};

struct literal {
    enum literaltag tag;

    union {
        u64 integer;
    } as;
};

enum binaryoptype {
    BIN_ADD,
    BIN_SUB,
    BIN_MUL,
    BIN_DIV,
};

struct binop {
    struct span span;
    enum binaryoptype op;
};

struct binary {
    struct binop op;
    struct expr *left;
    struct expr *right;
};

enum unaryoptype {
    UN_NEGATE,
};

struct unaryop {
    struct span span;
    enum unaryoptype op;
};

struct unary {
    struct unaryop op;
    struct expr *operand;
};

struct variable {
    const char *identifier;
};

struct varassign {
    struct expr *target;
    struct expr *value;
};

struct exprtype {
    enum exprtag tag;
    
    union {
        struct literal literal;
        struct binary binary;
        struct unary unary;
        struct variable variable;
        struct varassign varassign;
    } as;
};

struct expr {
    struct nodeid id;
    struct span span;
    struct exprtype type;
};

enum stmttag {
    STMT_BLOCK,
    STMT_EXPRSTMT,
    STMT_VAR_DECL,

    STMT_ERROR,
};

struct block_list {
    struct stmt **body;
    size_t length;
};

struct block {
    struct block_list stmts;
};

struct exprstmt {
    struct expr *expr;
};

struct vardecl {
    const char *name;
    enum type type_annotation;
    struct expr *initialiser;
};
struct stmttype {
    enum stmttag tag;
    union {
        struct block block;
        struct exprstmt exprstmt;
        struct vardecl vardecl;
    } as;
};

struct stmt {
    struct nodeid id;
    struct span span;
    struct stmttype type;
};

struct program {
    struct block_list body;
    struct arena arena;
};

#endif // CBB_PARSER_AST_H