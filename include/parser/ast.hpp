#ifndef CBB_PARSER_AST_HPP
#define CBB_PARSER_AST_HPP

#include <vector>
#include <string_view>
#include <memory>
#include <variant>

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

struct literal {
    std::variant<std::uint64_t> value;

    explicit literal(std::uint64_t value);
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

    binary(binop op, expr *left, expr *right);
};

struct unary {
    unaryop op;
    expr *operand;

    unary(unaryop op, expr *operand);
};

struct expr {
    nodeid id;
    core::span span;

    std::variant<literal, binary, unary, variable, var_assign, std::monostate> value;

    expr(nodeid id, core::span span, literal literal);
    expr(nodeid id, core::span span, binary binary);
    expr(nodeid id, core::span span, unary unary);
    expr(nodeid id, core::span span, variable variable);
    expr(nodeid id, core::span span, var_assign var_assign);
    expr(nodeid id, core::span span);
};

struct exprstmt {
    expr *expr;

    explicit exprstmt(struct expr *expr);
};

enum class type_syntax_type {
    // name,
    
    // no additional data is needed if the type is int
    int_,
};

struct type_syntax {
    type_syntax_type type;
    union {
        // std::string_view name;
    } as;
};

struct var_decl {
    std::string_view name;
    type_syntax type_annotation;
    expr *initialiser;

    var_decl(std::string_view name, type_syntax type_annotation, expr *initialiser);
};

struct block {
    std::vector<stmt*> body;    

    explicit block(std::vector<stmt*> body);
};

struct stmt {
    nodeid id;
    core::span span;

    std::variant<std::monostate, exprstmt, var_decl, block> value;

    stmt(nodeid id, core::span span, exprstmt exprstmt);
    stmt(nodeid id, core::span span, var_decl var_decl);
    stmt(nodeid id, core::span span, block block);
    stmt(nodeid id, core::span span);
};

struct program {
    std::unique_ptr<core::arena> arena;
    std::vector<stmt*> body;
};

} // namespace parser

} // namespace cbb

#endif // CBB_PARSER_AST_HPP