#include "analysis/resolution.hpp"

#include <format>

#include "core/overloaded.hpp"
#include "core/error.hpp"

namespace cbb {

namespace sema {

namespace ast = parser;

resolver::resolver(): current_local_id_(0) {}

localid resolver::declare_var(std::string_view name, core::span span, analysis_ctx &ctx) {
    if (ctx.scopes.back().variables.contains(name)) {
        error(std::format("redeclaration of identifier: '{}'", name), span, ctx);

        return localid::error();
    }

    localid id = current_local_id_;
    ctx.scopes.back().variables.emplace(name, id);

    ++current_local_id_;
    return id;
}

std::optional<localid> resolver::lookup_var(std::string_view name, analysis_ctx &ctx) {
    for (auto it = ctx.scopes.rbegin(); it != ctx.scopes.rend(); ++it) {
        for (auto [var_name, id] : it->variables) {
            if (name == var_name) {
                return id;
            } 
        }
    }
    return std::nullopt;
}

void resolver::enter_scope(analysis_ctx &ctx) {
    ctx.scopes.push_back(scope());
}

void resolver::exit_scope(analysis_ctx &ctx) {
    ctx.scopes.pop_back();
}

void resolver::resolve_stmt(const parser::stmt &stmt, analysis_ctx &ctx) {
    std::visit(core::overloaded {
        [&, this](const ast::var_decl &decl) {
            resolve_var_decl(decl, nodedata(stmt.id, stmt.span), ctx);
        },
        [&, this](const ast::block &block) {
            resolve_block(block, nodedata(stmt.id, stmt.span), ctx);
        },
        [&, this](const ast::exprstmt &exprstmt) {
            resolve_exprstmt(exprstmt, nodedata(stmt.id, stmt.span), ctx);
        },
        [](std::monostate) {
            UNREACHABLE();
        }
    }, stmt.value);
}

void resolver::resolve_block(const parser::block &block, nodedata, analysis_ctx &ctx) {
    enter_scope(ctx);

    for (auto it = block.body.begin(); it != block.body.end(); ++it) {
        resolve_stmt(**it, ctx);
    }

    exit_scope(ctx);
}

void resolver::resolve_exprstmt(const parser::exprstmt &exprstmt, nodedata, analysis_ctx &ctx) {
    resolve_expr(*exprstmt.expr, ctx);
}

void resolver::resolve_var_decl(const parser::var_decl &var_decl, nodedata data, analysis_ctx &ctx) {
    if (var_decl.initialiser != nullptr) {
        resolve_expr(*var_decl.initialiser, ctx);
    }

    localid id = declare_var(var_decl.name, data.span, ctx);

    ctx.variables.emplace(data.id, id);
}

void resolver::resolve_expr(const ast::expr &expr, analysis_ctx &ctx) {
    std::visit(core::overloaded {
        [&, this](const ast::var_assign &var_assign) {
            resolve_var_assign(var_assign, nodedata(expr.id, expr.span), ctx);
        },
        [&, this](const ast::variable &variable) {
            resolve_variable(variable, nodedata(expr.id, expr.span), ctx);
        },
        [&, this](const ast::binary &binary) {
            resolve_binary(binary, nodedata(expr.id, expr.span), ctx);
        },
        [&, this](const ast::unary &unary) {
            resolve_unary(unary, nodedata(expr.id, expr.span), ctx);
        },
        [&, this](const ast::literal &literal) {
            resolve_literal(literal, nodedata(expr.id, expr.span), ctx);
        },
        [](std::monostate) {
            UNREACHABLE();
        }
    }, expr.value);
}

void resolver::resolve_literal(const ast::literal &, nodedata, analysis_ctx &) {
    // no nothing because literals do not need any name resolution
    return;
}

void resolver::resolve_unary(const ast::unary &unary, nodedata, analysis_ctx &ctx) {
    resolve_expr(*unary.operand, ctx);
}

void resolver::resolve_binary(const ast::binary &binary, nodedata, analysis_ctx &ctx) {
    resolve_expr(*binary.left, ctx);
    resolve_expr(*binary.right, ctx);
}

void resolver::resolve_variable(const ast::variable &variable, nodedata data, analysis_ctx &ctx) {
    std::optional<localid> id = lookup_var(variable.name, ctx);

    if (!id) {
        error(std::format("use of undeclared identifier: '{}'", variable.name), data.span, ctx);
    }

    ctx.variables.emplace(data.id, *id);
}

void resolver::resolve_var_assign(const ast::var_assign &var_assign, nodedata, analysis_ctx &ctx) {
    resolve_expr(*var_assign.target, ctx);
    resolve_expr(*var_assign.value, ctx);
}

resolver::nodedata::nodedata(ast::nodeid id, core::span span): span(span), id(id) {}
    
} // namespace sema

    
} // namespace cbb
