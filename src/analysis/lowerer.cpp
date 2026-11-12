#include "analysis/lowerer.hpp"

#include <memory>
#include <utility>
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
    std::vector<stmt*> body;

    arena_ = &arena;

    for (auto it = program.body.begin(); it != program.body.end(); ++it) {
        body.push_back(lower_stmt(**it, ctx));
    }

    sema::program hir(std::move(arena), std::move(body));

    return hir;
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
        [](std::monostate) -> sema::stmt* {
            UNREACHABLE();
        }
    }, stmt.value);     

    return hir_stmt;
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
        [](std::monostate) -> sema::expr* {
            UNREACHABLE();
        },
    }, expr.value);

    return hir_expr;
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
