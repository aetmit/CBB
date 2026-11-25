#ifndef CBB_PARSER_AST_HPP
#define CBB_PARSER_AST_HPP

#include <vector>
#include <string>
#include <string_view>
#include <optional>
#include <memory>
#include <variant>
#include <ostream>
#include <functional>

#include <cstddef>
#include <cstdint>

#include "core/arena.hpp"
#include "core/span.hpp"

namespace cbb {

namespace parser {

struct nodeid {
    std::size_t value;

    explicit constexpr nodeid(std::size_t value): value(value) {}

    bool operator==(const nodeid&) const = default;
};

struct functionid {
    std::size_t value;

    explicit constexpr functionid(std::size_t value): value(value) {}

    bool operator==(const functionid&) const = default;
};

} // namespace parser

} // namespace cbb

template <>
struct std::hash<cbb::parser::nodeid> {
    std::size_t operator()(const cbb::parser::nodeid &id) const noexcept {
        return std::hash<std::size_t>{}(id.value);
    }
};

template <>
struct std::hash<cbb::parser::functionid> {
    std::size_t operator()(const cbb::parser::functionid &id) const noexcept {
        return std::hash<std::size_t>{}(id.value);
    }
};

namespace cbb {

namespace parser {

struct expr;
struct stmt;

struct literal {
    std::variant<std::uint64_t, bool> value;

    explicit literal(std::uint64_t value);
    explicit literal(bool value);
};

struct variable {
    std::string_view name;   
    
    explicit variable(std::string_view name);
};

struct var_assign {
    // target is verified to be an l-value in the analysis modules
    expr *target;
    expr *value;

    var_assign(expr *target, expr *value);
};

enum class binaryop_type {
    add,
    sub,
    mul,
    div,

    logical_and,
    logical_or,
    
    equal,
    not_equal,

    greater,
    greater_eq,
    less,
    less_eq,
};

enum class unaryop_type {
    negate,
    logical_not,
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

struct call {
    expr *callee;
    
    std::vector<expr*> arguments;

    call(expr *callee, std::vector<expr*> arguments);
};

struct expr {
    nodeid id;
    core::span span;

    std::variant<std::monostate, literal, binary, unary, variable, var_assign, call> value;

    expr(nodeid id, core::span span, literal literal);
    expr(nodeid id, core::span span, binary binary);
    expr(nodeid id, core::span span, unary unary);
    expr(nodeid id, core::span span, variable variable);
    expr(nodeid id, core::span span, var_assign var_assign);
    expr(nodeid id, core::span span, call call);
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
    //same for bool
    bool_,
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

struct return_stmt {
    expr* value; 

    explicit return_stmt(expr* value);
};

struct if_stmt {
    expr *condition;
    // stmt and not block because the if body doesn't need to be a block
    stmt *body;

    // nullptr if the else_clause doesn't exist
    stmt *else_clause;

    if_stmt(expr *condition, stmt *body, stmt *else_clause);
};

struct stmt {
    nodeid id;
    core::span span;

    std::variant<std::monostate, exprstmt, var_decl, block, return_stmt, if_stmt> value;

    stmt(nodeid id, core::span span, exprstmt exprstmt);
    stmt(nodeid id, core::span span, var_decl var_decl);
    stmt(nodeid id, core::span span, block block);
    stmt(nodeid id, core::span span, return_stmt return_stmt);
    stmt(nodeid id, core::span span, if_stmt if_stmt);
    stmt(nodeid id, core::span span);
};

// we do this because a block itself does not contain and id or span
struct function_block {
    nodeid id;
    core::span span;
    block body;

    function_block(nodeid id, core::span span, block block);
};

struct param {
    nodeid id;
    std::string_view name;
    type_syntax type;
    core::span span;

    param(nodeid id, std::string_view name, type_syntax type, core::span span);
};

struct function {
    functionid id;

    core::span span;

    std::string_view name;
    type_syntax return_type;
    // optional because we could have a function declaration but not have the definition
    std::optional<function_block> body;

    std::vector<param> params;

    function(functionid id, core::span span, std::string_view name, type_syntax return_type, std::optional<function_block> body, std::vector<param> params);
    // for functions with errors
    function(functionid id, core::span span);
};

struct program {
    // pointer here because the arena itself is non moveable and non copyable which
    // makes doing some stuff not very intuative
    // with unique_ptr we can then sorta move the arena but the allocation stays the same
    // its just the pointer that gets moved
    std::unique_ptr<core::arena> arena;
    std::vector<function*> body;
};

// these overloads reconstruct the ast back into source code 
// we are able to do this because the ast contans enough information
// to do so
// doesnt print the internal fields of each node
// for that you should use ast_formatter::format() and then print the 
// string that it returns
std::ostream &operator<<(std::ostream &os, const program &program);
std::ostream &operator<<(std::ostream &os, const stmt &stmt);
std::ostream &operator<<(std::ostream &os, const expr &expr);
std::ostream &operator<<(std::ostream &os, const block &block);
std::ostream &operator<<(std::ostream &os, const var_decl &var_decl);
std::ostream &operator<<(std::ostream &os, const type_syntax &type_syntax);
std::ostream &operator<<(std::ostream &os, const exprstmt &exprstmt);
std::ostream &operator<<(std::ostream &os, const unary &unary);
std::ostream &operator<<(std::ostream &os, const binary &binary);
std::ostream &operator<<(std::ostream &os, const unaryop_type &unaryop_type);
std::ostream &operator<<(std::ostream &os, const binaryop_type &binaryop_type);
std::ostream &operator<<(std::ostream &os, const var_assign &var_assign);
std::ostream &operator<<(std::ostream &os, const variable &variable);
std::ostream &operator<<(std::ostream &os, const literal &literal);
std::ostream &operator<<(std::ostream &os, const function &function);
std::ostream &operator<<(std::ostream &os, const function_block &function_block);
std::ostream &operator<<(std::ostream &os, const param &param);
std::ostream &operator<<(std::ostream &os, const return_stmt &return_stmt);
std::ostream &operator<<(std::ostream &os, const nodeid &id);
std::ostream &operator<<(std::ostream &os, const call &call);
std::ostream &operator<<(std::ostream &os, const if_stmt &if_stmt);

class ast_formatter {
public:
    ast_formatter() = default;

    std::string format(const program &program);

private:
    void write_indent(std::ostream &os, std::size_t indent) const;
    void write_span(std::ostream &os, const core::span &span) const;
    void format_type_syntax(std::ostream &os, const type_syntax &type, std::size_t indent) const;
    void format_expression(std::ostream &os, const expr *expression, std::size_t indent) const;
    void format_statement(std::ostream &os, const stmt *statement, std::size_t indent) const;
    void format_function(std::ostream &os, const function *function, std::size_t indent) const;
    void format_function_block(std::ostream &os, const function_block &block, std::size_t indent) const;
};


} // namespace parser

} // namespace cbb

#endif // CBB_PARSER_AST_HPP