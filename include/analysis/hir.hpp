#ifndef CBB_ANALYSIS_HIR_HPP
#define CBB_ANALYSIS_HIR_HPP

#include <memory>
#include <vector>
#include <optional>
#include <variant>

#include "analysis/ids.hpp"
#include "analysis/type.hpp"
#include "core/arena.hpp"

namespace cbb {

namespace sema {

struct expr;
struct stmt;

struct literal {
    std::variant<std::int32_t, bool> value;

    explicit literal(std::int32_t value);
    explicit literal(bool value);
};

struct variable {
    localid id;

    explicit variable(localid id);
};

struct function_ref {
    functionid id;

    explicit function_ref(functionid id);
};

struct var_assign {
    expr *target;
    expr *value;

    var_assign(expr *target, expr *value);
};

enum class binarytype {
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

struct binary {
    binarytype op;
    expr *left;
    expr *right;

    binary(binarytype op, expr *left, expr *right);
};

enum class unarytype {
    negate,

    logical_not,
};

struct unary {
    unarytype op;
    expr *operand;

    unary(unarytype op, expr *operand);
};

struct call {
    expr *callee;
    std::vector<expr*> arguments;

    call(expr *callee, std::vector<expr*> arguments);
};

struct expr {
    hirid id;
    type_info type;

    std::variant<unary, binary, var_assign, variable, literal, call, function_ref> value;   

    expr(hirid id, type_info type, unary unary);
    expr(hirid id, type_info type, binary binary);
    expr(hirid id, type_info type, var_assign var_assign);
    expr(hirid id, type_info type, variable varialbe);
    expr(hirid id, type_info type, literal literal);
    expr(hirid id, type_info type, call call);
    expr(hirid id, type_info type, function_ref function_ref);
};

struct exprstmt {
    expr *expr;

    explicit exprstmt(struct expr *expr);
};

struct block {
    std::vector<stmt*> body;

    explicit block(std::vector<stmt*> body);
};

struct var_decl {
    localid id;   
    type_info type;
    expr *initaliser;

    var_decl(localid id, type_info type, expr *initaliser);
};

struct return_stmt {
    std::optional<expr*> value;

    explicit return_stmt(std::optional<expr*> value);
};

struct if_stmt {
    expr *condition;
    stmt *body;   

    // nullptr if no else is present
    stmt *else_clause;

    if_stmt(expr *condition, stmt *body, stmt *else_clause);
};

struct stmt {
    hirid id;

    std::variant<exprstmt, var_decl, block, return_stmt, if_stmt> value;

    stmt(hirid id, exprstmt exprstmt);
    stmt(hirid id, var_decl var_decl);
    stmt(hirid id, block block);
    stmt(hirid id, return_stmt return_stmt);
    stmt(hirid id, if_stmt if_stmt);
};

struct function_block {
    hirid id;
    block body;

    function_block(hirid id, block block);
};

struct param {
    // id of the actaul param node
    hirid id;
    // id of the actual variable
    localid local_id;
    type type;

    param(hirid id, localid local_id, enum type type);
};

struct function {
    functionid id;
    type return_type;

    std::optional<function_block> body;
    std::vector<param> params;

    function(functionid id, type return_type, std::optional<function_block> body, std::vector<param> params);
};

struct program {
    // pointer here because the arena itself is non moveable and non copyable which
    // makes doing some stuff not very intuative
    // with unique_ptr we can then sorta move the arena but the allocation stays the same
    // its just the pointer that gets moved
    std::unique_ptr<core::arena> arena;
    std::vector<function*> body;

    program(std::unique_ptr<core::arena> arena, std::vector<function*> body);
};

class hir_formatter {
public:
    hir_formatter() = default;  

    std::string format(const program &program);
private:
    void indent(std::ostream &out, std::size_t depth);

    void format_expr(std::ostream &out, const expr *node, std::size_t depth);
    void format_stmt(std::ostream &out, const stmt *node, std::size_t depth);

    const char *type_name(type t);
    const char *value_cat_name(value_cat cat);
    const char *binary_name(binarytype op);
    const char *unary_name(unarytype op);
};
    
} // namespace sema

    
} // namespace cbb


#endif // CBB_ANALYSIS_HIR_HPP