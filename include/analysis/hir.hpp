#ifndef CBB_ANALYSIS_HIR_HPP
#define CBB_ANALYSIS_HIR_HPP

#include <memory>
#include <vector>
#include <variant>

#include "analysis/ids.hpp"
#include "analysis/type.hpp"
#include "core/arena.hpp"

namespace cbb {

namespace sema {

struct expr;
struct stmt;

struct literal {
    std::variant<std::int32_t> value;

    explicit literal(std::int32_t value);
};

struct variable {
    localid id;

    explicit variable(localid id);
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
};

struct binary {
    binarytype op;
    expr *left;
    expr *right;

    binary(binarytype op, expr *left, expr *right);
};

enum class unarytype {
    negate,
};

struct unary {
    unarytype op;
    expr *operand;

    unary(unarytype op, expr *operand);
};

struct expr {
    hirid id;
    type_info type;

    std::variant<unary, binary, var_assign, variable, literal> value;   

    expr(hirid id, type_info type, unary unary);
    expr(hirid id, type_info type, binary binary);
    expr(hirid id, type_info type, var_assign var_assign);
    expr(hirid id, type_info type, variable varialbe);
    expr(hirid id, type_info type, literal literal);
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

struct stmt {
    hirid id;

    std::variant<exprstmt, var_decl, block> value;

    stmt(hirid id, exprstmt exprstmt);
    stmt(hirid id, var_decl var_decl);
    stmt(hirid id, block block);

};

struct program {
    // pointer here because the arena itself is non moveable and non copyable which
    // makes doing some stuff not very intuative
    // with unique_ptr we can then sorta move the arena but the allocation stays the same
    // its just the pointer that gets moved
    std::unique_ptr<core::arena> arena;
    std::vector<stmt*> body;

    program(std::unique_ptr<core::arena> arena, std::vector<stmt*> body);
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