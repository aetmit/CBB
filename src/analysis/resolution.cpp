#include "analysis/resolution.hpp"

#include <format>
#include <ranges>
#include <utility>

#include "core/overloaded.hpp"
#include "core/error.hpp"

namespace cbb {

namespace sema {

namespace ast = parser;

signature::signature(type return_type, std::vector<type> param_types): return_type(return_type), param_types(std::move(param_types)) {}

function_info::function_info(signature sig, bool defined): sig(sig), defined(defined) {}

resolver::resolver(): current_local_id_(0), current_function_id_(0), in_fn_(false) {}

void resolver::resolve(const ast::program &program, context &ctx) {
    enter_scope(ctx);
    for (auto it = program.body.begin(); it != program.body.end(); ++it)  {

        declare_func(**it, ctx);

        if ((*it)->body) {
            resolve_function(**it, ctx);
        }
    }

    exit_scope(ctx);
}

void resolver::resolve_function(const ast::function &function, context &ctx) {
    in_fn_ = true;

    enter_scope(ctx);

    declare_fn_params(function.params, ctx);

    for (auto it = function.body.value().body.body.begin(); it != function.body.value().body.body.end(); ++it) {
        resolve_stmt(**it, ctx);
    }

    exit_scope(ctx);

    in_fn_ = false;
}

void resolver::declare_fn_params(const std::vector<ast::param> &params, context &ctx) {
    // assume that a scope has been entered for us

    // use ranges here because i cannot be bothered to type out another iterator for loop
    // even tho this comment was prolly longer than that
    std::ranges::for_each(params, [&](const ast::param &param){
        localid id = declare_var(param.name, param.span, ctx);
        ctx.variables.emplace(param.id, id);
    });
}

signature resolver::get_fn_signature(const ast::function &fn) const {
    // use ranges here because i am too used to rust iterators and cant be 
    // bothered to think of another way to do ts
    std::vector<type> param_types = fn.params | std::views::transform([&](const auto &param) {
        return resolve_type(param.type);
    }) | std::ranges::to<std::vector>();

    type return_type = resolve_type(fn.return_type);
    return signature(return_type, std::move(param_types));
}

type resolver::resolve_type(const ast::type_syntax &type) const {
    switch (type.type) {
        case ast::type_syntax_type::int_:
            return type::int_;    
        case ast::type_syntax_type::bool_:
            return type::bool_;
    }
}

localid resolver::declare_var(std::string_view name, core::span span, context &ctx) {
    if (ctx.scopes.back().variables.contains(name)) {
        error(std::format("redeclaration of identifier: '{}'", name), span, ctx);

        return localid::error();
    }

    // at file scope
    if (ctx.scopes.size() == 1) {
        if (ctx.scopes.back().functions.contains(name)) {
            error(std::format("redeclaration of identifer: {}", name), span, ctx);

            return localid::error();
        }
    }

    localid id = current_local_id_;
    ctx.scopes.back().variables.emplace(name, id);

    ++current_local_id_;
    return id;
}

std::optional<functionid> resolver::lookup_func(std::string_view name, context &ctx) {
    for (auto it = ctx.scopes.rbegin(); it != ctx.scopes.rend(); ++it) {
        for (const auto& [func_name, id] : it->functions) {
            if (name == func_name) {
                return id;
            }
        }
    }

    return std::nullopt;
}

functionid resolver::declare_func(const ast::function &fn, context &ctx) {
    if (ctx.scopes.back().variables.contains(fn.name)) {
        error(std::format("redeclaration of identifier: '{}'", fn.name), fn.span, ctx);
        return functionid::error();
    }

    auto res = lookup_func(fn.name, ctx);
    // if res is std::nullopt then we declare the function
    if (!res) {
        functionid id = current_function_id_;
        ctx.scopes.back().functions.emplace(fn.name, id);
        ++current_function_id_;
        signature sig = get_fn_signature(fn);
        function_info info(sig, static_cast<bool>(fn.body));
        ctx.func_info.emplace(id, info);
        ctx.functions.emplace(fn.id, id);
        return id;
    }

    functionid id = *res;
    function_info info = ctx.func_info.at(id);
    signature sig = get_fn_signature(fn);
    if (sig != info.sig) {
        error(std::format("conflicting types for '{}'", fn.name), fn.span, ctx);
        return functionid::error();
    }
    if (fn.body) {
        if (info.defined) {
            error(std::format("redefinition of '{}'", fn.name), fn.span, ctx);
            return functionid::error();
        }
        ctx.func_info.at(id).defined = true;
    }
    ctx.functions.emplace(fn.id, id);
    return id;
}

std::optional<localid> resolver::lookup_var(std::string_view name, context &ctx) {
    for (auto it = ctx.scopes.rbegin(); it != ctx.scopes.rend(); ++it) {
        for (const auto& [var_name, id] : it->variables) {
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
        [&](const ast::return_stmt &return_) {
            resolve_return_stmt(return_, nodedata(stmt.id, stmt.span), ctx);
        },
        [](std::monostate) {
            UNREACHABLE();
        }
    }, stmt.value);
}

void resolver::resolve_return_stmt(const parser::return_stmt& return_stmt, nodedata data, context &ctx) {
    if (!in_fn_) {
        error("use of 'return' statement outside function", data.span, ctx);
        return;
    } 

    if (return_stmt.value) {
        resolve_expr(**return_stmt.value, ctx);
    }
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
        [&](const ast::call &call) {
            resolve_call(call, nodedata(expr.id, expr.span), ctx);
        },
        [](std::monostate) {
            UNREACHABLE();
        }
    }, expr.value);
}

void resolver::resolve_call(const ast::call &call, nodedata, context &ctx) {
    resolve_expr(*call.callee, ctx);
    for (auto it = call.arguments.begin(); it != call.arguments.end(); ++it) {
        resolve_expr(**it, ctx);
    }
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
    if (auto id = lookup_var(variable.name, ctx)) {
        ctx.variables.emplace(data.id, *id);
        return;
    }

    if (auto id = lookup_func(variable.name, ctx)) {
        ctx.function_refs.emplace(data.id, *id);
        return;
    }

    error(
        std::format("use of undeclared identifier: '{}'", variable.name),
        data.span,
        ctx
    );
}

void resolver::resolve_var_assign(const ast::var_assign &var_assign, nodedata, context &ctx) {
    resolve_expr(*var_assign.value, ctx);
    resolve_expr(*var_assign.target, ctx);
}

resolver::nodedata::nodedata(ast::nodeid id, core::span span): span(span), id(id) {}
    
} // namespace sema

    
} // namespace cbb
