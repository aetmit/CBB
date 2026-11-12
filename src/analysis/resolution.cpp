#include "analysis/resolution.hpp"

#include <format>

#include "core/overloaded.hpp"
#include "core/error.hpp"

namespace cbb {

namespace sema {

namespace ast = parser;

resolver::resolver(): current_local_id_(0) {}

void resolver::resolve(const ast::program &program, context &ctx) {
    for (auto it = program.body.begin(); it != program.body.end(); ++it)  {
        resolve_stmt(**it, ctx);
    }
}

localid resolver::declare_var(std::string_view name, core::span span, context &ctx) {
    if (ctx.scopes.back().variables.contains(name)) {
        error(std::format("redeclaration of identifier: '{}'", name), span, ctx);

        return localid::error();
    }

    localid id = current_local_id_;
    ctx.scopes.back().variables.emplace(name, id);

    ++current_local_id_;
    return id;
}

std::optional<localid> resolver::lookup_var(std::string_view name, context &ctx) {
    for (auto it = ctx.scopes.rbegin(); it != ctx.scopes.rend(); ++it) {
        for (auto [var_name, id] : it->variables) {
            if (name == var_name) {
                return id;
            } 
        }
    }
    return std::nullopt;
}

void resolver::enter_scope(context &ctx) {
    ctx.scopes.push_back(scope());
}

void resolver::exit_scope(context &ctx) {
    ctx.scopes.pop_back();
}

void resolver::resolve_stmt(const parser::stmt &stmt, context &ctx) {
    std::visit(core::overloaded {
        [&](const ast::var_decl &decl) {
            resolve_var_decl(decl, nodedata(stmt.id, stmt.span), ctx);
        },
        [&](const ast::block &block) {
            resolve_block(block, nodedata(stmt.id, stmt.span), ctx);
        },
        [&](const ast::exprstmt &exprstmt) {
            resolve_exprstmt(exprstmt, nodedata(stmt.id, stmt.span), ctx);
        },
        [](std::monostate) {
            UNREACHABLE();
        }
    }, stmt.value);
}

void resolver::resolve_block(const parser::block &block, nodedata, context &ctx) {
    enter_scope(ctx);

    for (auto it = block.body.begin(); it != block.body.end(); ++it) {
        resolve_stmt(**it, ctx);
    }

    exit_scope(ctx);
}

void resolver::resolve_exprstmt(const parser::exprstmt &exprstmt, nodedata, context &ctx) {
    resolve_expr(*exprstmt.expr, ctx);
}

void resolver::resolve_var_decl(const parser::var_decl &var_decl, nodedata data, context &ctx) {
    if (var_decl.initialiser != nullptr) {
        resolve_expr(*var_decl.initialiser, ctx);
    }

    localid id = declare_var(var_decl.name, data.span, ctx);

    ctx.variables.emplace(data.id, id);
}

void resolver::resolve_expr(const ast::expr &expr, context &ctx) {
    std::visit(core::overloaded {
        [&](const ast::var_assign &var_assign) {
            resolve_var_assign(var_assign, nodedata(expr.id, expr.span), ctx);
        },
        [&](const ast::variable &variable) {
            resolve_variable(variable, nodedata(expr.id, expr.span), ctx);
        },
        [&](const ast::binary &binary) {
            resolve_binary(binary, nodedata(expr.id, expr.span), ctx);
        },
        [&](const ast::unary &unary) {
            resolve_unary(unary, nodedata(expr.id, expr.span), ctx);
        },
        [&](const ast::literal &literal) {
            resolve_literal(literal, nodedata(expr.id, expr.span), ctx);
        },
        [](std::monostate) {
            UNREACHABLE();
        }
    }, expr.value);
}

void resolver::resolve_literal(const ast::literal &, nodedata, context &) {
    // no nothing because literals do not need any name resolution
    return;
}

void resolver::resolve_unary(const ast::unary &unary, nodedata, context &ctx) {
    resolve_expr(*unary.operand, ctx);
}

void resolver::resolve_binary(const ast::binary &binary, nodedata, context &ctx) {
    resolve_expr(*binary.left, ctx);
    resolve_expr(*binary.right, ctx);
}

void resolver::resolve_variable(const ast::variable &variable, nodedata data, context &ctx) {
    std::optional<localid> id = lookup_var(variable.name, ctx);

    if (!id) {
        error(std::format("use of undeclared identifier: '{}'", variable.name), data.span, ctx);
    }

    ctx.variables.emplace(data.id, *id);
}

void resolver::resolve_var_assign(const ast::var_assign &var_assign, nodedata, context &ctx) {
    resolve_expr(*var_assign.target, ctx);
    resolve_expr(*var_assign.value, ctx);
}

resolver::nodedata::nodedata(ast::nodeid id, core::span span): span(span), id(id) {}
    
} // namespace sema

    
} // namespace cbb
