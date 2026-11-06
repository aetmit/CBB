#ifndef CBB_PARSER_AST_HPP
#define CBB_PARSER_AST_HPP

#include <vector>
#include <string_view>
#include <memory>

#include <cstddef>
#include <cstdint>

#include "core/arena.hpp"
#include "core/span.hpp"

namespace cbb {

namespace parser {

struct nodeid {
    std::size_t value;

    explicit constexpr nodeid(std::size_t value): value(value) {}
};

struct expr;
struct stmt;

enum class expr_type {
    binary,
    unary,
    literal,
    variable,
    var_assign,

    error,
};

enum class literal_type {
    integer,
};

struct literal {
    literal_type type;
    union {
        std::uint64_t integer;
    } as;
};

struct variable {
    std::string_view name;   
};

// target is verified to be an l-value in the analysis modules
struct var_assign {
    expr *target;
    expr *value;
};

enum class binaryop_type {
    add,
    sub,
    mul,
    div,
};

enum class unaryop_type {
    negate,
};

using binop = core::spanned<binaryop_type>;
using unaryop = core::spanned<unaryop_type>;

struct binary {
    binop op;
    expr *left;
    expr *right;
};

struct unary {
    binop op;
    expr *operand;
};

struct expr {
    nodeid id;
    expr_type type;
    core::span span;
    union {
        literal literal;
        binary binary;
        unary unary;
        variable variable;
        var_assign var_assign;
    } as;

    expr(nodeid id, core::span span, literal literal);
    expr(nodeid id, core::span span, binary binary);
    expr(nodeid id, core::span span, unary unary);
    expr(nodeid id, core::span span, variable variable);
    expr(nodeid id, core::span span, var_assign var_assign);
    expr(nodeid id, core::span span);
};

enum class stmt_type {
    exprstmt,
    var_decl,
};

struct exprstmt {
    expr *expr;
};

enum class type_syntax_type {
    name,
};

struct type_syntax {
    type_syntax_type type;
    union {
        std::string_view name;
    } as;
};

struct var_decl {
    std::string_view name;
    type_syntax type_annotation;
    expr *initialiser;
};

struct stmt {
    stmt_type type;
    core::span span;
    nodeid id;
    union {
        exprstmt exprstmt;
        var_decl var_decl;
    } as;
};

struct program {
    std::unique_ptr<core::arena> arena;
    std::vector<stmt*> body;
};

} // namespace parser

} // namespace cbb

#endif // CBB_PARSER_AST_HPP