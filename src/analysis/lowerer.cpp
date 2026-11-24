#include "analysis/lowerer.hpp"

#include <memory>
#include <utility>
#include <ranges>
#include <vector>
#include <variant>
#include <optional>

#include <cstdint>

#include "core/arena.hpp"
#include "core/error.hpp"
#include "core/overloaded.hpp"

namespace cbb {

namespace sema {

namespace ast = parser;

lowerer::lowerer(): current_hirid_(0), arena_(nullptr) {}

program lowerer::lower(const ast::program &program, context &ctx) {

    auto arena = std::make_unique<core::arena>();
    std::vector<function*> body;

    arena_ = &arena;

    for (auto it = program.body.begin(); it != program.body.end(); ++it) {
        body.push_back(lower_function(**it, ctx));
    }

    sema::program hir(std::move(arena), std::move(body));

    return hir;
}

function *lowerer::lower_function(const ast::function &function, context &ctx) {
    functionid id = ctx.functions.at(function.id);
    function_info info = ctx.func_info.at(id);

    std::vector<param> params;

    for (auto it = function.params.begin(); it != function.params.end(); ++it) {
        hirid hir_id = next_hirid();
        localid local_id = ctx.variables.at(it->id);
        type type = ctx.var_types.at(local_id).type;
        params.emplace_back(hir_id, local_id, type);
    }    

    if (!function.body) {
        struct function *hir_function = alloc_function(id, info.sig.return_type, std::nullopt, std::move(params));
        return hir_function;
    }

    struct function *hir_function = alloc_function(id, info.sig.return_type, lower_function_body(*function.body, ctx), std::move(params));
    return hir_function;
}

function_block lowerer::lower_function_body(const ast::function_block &body, context &ctx) {
    std::vector<stmt*> stmts;
    for (auto it = body.body.body.begin(); it != body.body.body.end(); ++it) {
        stmts.push_back(lower_stmt(**it, ctx));
    } 
    
    block block(std::move(stmts));

    return function_block(next_hirid(), block);
}

function *lowerer::alloc_function(functionid id, type return_type, std::optional<function_block> body, std::vector<param> params) {
    void *memory = (*arena_)->allocate(sizeof(function), alignof(function));

    function *function = new (memory) struct function(id, return_type, body, std::move(params));

    return function;
}

stmt *lowerer::lower_stmt(const ast::stmt &stmt, context &ctx) {
    sema::stmt *hir_stmt = std::visit(core::overloaded {
        [&](const ast::exprstmt &exprstmt) -> sema::stmt* {
            return lower_exprstmt(exprstmt, ctx);
        },
        [&](const ast::block &block) -> sema::stmt* {
            return lower_block(block, ctx);
        },
        [&](const ast::var_decl &var_decl) -> sema::stmt* {
            return lower_var_decl(var_decl, stmt.id, ctx);
        },
        [&](const ast::return_stmt &return_stmt) -> sema::stmt* {
            return lower_return_stmt(return_stmt, ctx);
        },
        [](std::monostate) -> sema::stmt* {
            UNREACHABLE();
        }
    }, stmt.value);     

    return hir_stmt;
}

stmt *lowerer::lower_return_stmt(const ast::return_stmt &return_stmt, context &ctx) {
    if (return_stmt.value) {
        expr *expr = lower_expr(*return_stmt.value, ctx);
        struct return_stmt return_(expr);
        stmt *stmt = alloc_stmt(return_);
        return stmt;
    }
    struct return_stmt return_(std::nullopt);
    stmt *stmt = alloc_stmt(return_);
    return stmt;
}

stmt *lowerer::lower_var_decl(const ast::var_decl &var_decl, ast::nodeid nodeid, context &ctx) {
    localid id = ctx.variables.at(nodeid);

    type_info type = ctx.var_types.at(id);

    sema::expr *initaliser;

    if (var_decl.initialiser == nullptr) {
        initaliser = nullptr;
    } else {
        initaliser = lower_expr(*var_decl.initialiser, ctx);
    }

    sema::var_decl hir_vardecl(id, type, initaliser);

    sema::stmt *hir_stmt = alloc_stmt(hir_vardecl);

    return hir_stmt;
}

stmt *lowerer::lower_exprstmt(const ast::exprstmt &exprstmt, context &ctx) {
    expr *expr = lower_expr(*exprstmt.expr, ctx);
    
    struct exprstmt hir_exprstmt(expr);

    stmt *hir_stmt = alloc_stmt(hir_exprstmt);

    return hir_stmt;
}

stmt *lowerer::lower_block(const ast::block &block, context &ctx) {
    std::vector<sema::stmt*> body;
    for (auto it = block.body.begin(); it != block.body.end(); ++it) {
        body.push_back(lower_stmt(**it, ctx));
    }

    sema::block hir_block(std::move(body));

    sema::stmt *hir_stmt = alloc_stmt(hir_block);

    return hir_stmt;
}

expr *lowerer::lower_expr(const ast::expr &expr, context &ctx) {
    sema::expr *hir_expr = std::visit(core::overloaded {
        [&](const ast::literal &literal) -> sema::expr* {
            return lower_literal(literal, expr.id, ctx);
        },
        [&](const ast::binary &binary) -> sema::expr* {
            return lower_binary(binary, expr.id, ctx);
        },
        [&](const ast::unary &unary) -> sema::expr* {
            return lower_unary(unary, expr.id, ctx);
        },
        [&](const ast::variable &variable) -> sema::expr* {
            return lower_variable(variable, expr.id, ctx);
        },
        [&](const ast::var_assign &var_assign) -> sema::expr* {
            return lower_var_assign(var_assign, expr.id, ctx);
        },
        [&](const ast::call &call) -> sema::expr* {
            return lower_call(call, expr.id, ctx);
        },
        [](std::monostate) -> sema::expr* {
            UNREACHABLE();
        },
    }, expr.value);

    return hir_expr;
}

expr *lowerer::lower_call(const ast::call &call, ast::nodeid, context &ctx) {
    functionid ref = ctx.function_refs.at(call.callee->id);
    function_info info = ctx.func_info.at(ref);
    expr *callee = alloc_expr(
        type_info(info.sig.return_type, value_cat::rvalue),
        sema::function_ref(ref)
    );

    std::vector<expr*> args = call.arguments | std::views::transform([&](const auto &arg) {
        return lower_expr(*arg, ctx);
    }) | std::ranges::to<std::vector>();

    struct call call_expr(callee, args);
    expr *expr = alloc_expr(type_info(info.sig.return_type, value_cat::rvalue), call_expr);
    return expr;
}

expr *lowerer::lower_var_assign(const ast::var_assign &var_assign, ast::nodeid id, context &ctx) {
    sema::expr *hir_value = lower_expr(*var_assign.value, ctx);
    sema::expr *hir_target = lower_expr(*var_assign.target, ctx);

    sema::var_assign hir_var_assign(hir_target, hir_value);

    type_info type = ctx.types.at(id);

    sema::expr *hir_expr = alloc_expr(type, hir_var_assign);

    return hir_expr;
}

expr *lowerer::lower_variable(const ast::variable&, ast::nodeid id, context &ctx) {
    localid var_id = ctx.variables.at(id);

    sema::variable hir_variable(var_id);

    type_info type = ctx.types.at(id);

    sema::expr *hir_expr = alloc_expr(type, hir_variable);

    return hir_expr;
}

expr *lowerer::lower_unary(const ast::unary &unary, ast::nodeid id, context &ctx) {
    sema::unarytype op = lower_unaryop(unary.op.node);

    sema::expr *hir_operand = lower_expr(*unary.operand, ctx);

    sema::unary hir_unary(op, hir_operand);

    type_info type = ctx.types.at(id);

    sema::expr *hir_expr = alloc_expr(type, hir_unary);

    return hir_expr;
}

unarytype lowerer::lower_unaryop(ast::unaryop_type op) {
    switch (op) {
        case ast::unaryop_type::negate:
            return unarytype::negate;
        case ast::unaryop_type::logical_not:
            return unarytype::logical_not;
    }
}

expr *lowerer::lower_binary(const ast::binary &binary, ast::nodeid id, context &ctx) {
    binarytype op = lower_binaryop(binary.op.node);

    sema::expr *hir_left = lower_expr(*binary.left, ctx);
    sema::expr *hir_right = lower_expr(*binary.right, ctx);
    
    sema::binary hir_binary(op, hir_left, hir_right);

    type_info type = ctx.types.at(id);

    sema::expr *hir_expr = alloc_expr(type, hir_binary);

    return hir_expr;
}

binarytype lowerer::lower_binaryop(ast::binaryop_type op) {
    switch (op) {
        case ast::binaryop_type::add:
            return binarytype::add;
        case ast::binaryop_type::sub:
            return binarytype::sub;
        case ast::binaryop_type::mul:
            return binarytype::mul;
        case ast::binaryop_type::div:
            return binarytype::div;
        case ast::binaryop_type::logical_and:
            return binarytype::logical_and;
        case ast::binaryop_type::logical_or:
            return binarytype::logical_or;
        case ast::binaryop_type::equal:
            return binarytype::equal;
        case ast::binaryop_type::not_equal:
            return binarytype::not_equal;
        case ast::binaryop_type::greater:
            return binarytype::greater;
        case ast::binaryop_type::greater_eq:
            return binarytype::greater_eq;
        case ast::binaryop_type::less:
            return binarytype::less;
        case ast::binaryop_type::less_eq:
            return binarytype::less_eq;
    }
}

expr *lowerer::lower_literal(const ast::literal &literal, ast::nodeid id, context &ctx) {
    std::optional<struct literal> hir_literal;

    type_info type = ctx.types.at(id);

    std::visit(core::overloaded {
        [&hir_literal](std::int32_t integer) {
            hir_literal.emplace(integer);
        },
    }, literal.value);

    sema::expr *hir_expr = alloc_expr(type, *hir_literal);

    return hir_expr;
}

hirid lowerer::next_hirid() {
    auto id = current_hirid_;
    ++current_hirid_.value;
    return id;
}
    
} // namespace sema
    
} // namespace cbb
