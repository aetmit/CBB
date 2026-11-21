#include "analysis/type_checker.hpp"

#include <limits>
#include <ranges>
#include <sstream>

#include <cassert>

#include "core/overloaded.hpp"
#include "core/error.hpp"

namespace cbb {

namespace sema {

namespace ast = parser;

type_checker::type_checker(): function_context_(std::nullopt) {}

void type_checker::check(const ast::program &program, context &ctx) {
    for (auto it = program.body.begin(); it != program.body.end(); ++it) {
        check_function(**it, ctx);
    }
}

void type_checker::check_function(const ast::function &function, context &ctx) {
    if (function.body) {
        std::ranges::for_each(function.body.value().body.body, [&](const auto &stmt) {

            std::ranges::for_each(function.params, [&](const auto &param) {
                localid id = ctx.variables.at(param.id);
                type param_type = get_annotation_type(param.type);

                ctx.var_types.emplace(id, type_info(param_type, value_cat::lvalue));
            });

            check_stmt(*stmt, ctx);
        });
    }
}

void type_checker::check_stmt(const ast::stmt &stmt, context &ctx) {
    std::visit(core::overloaded {
        [&](const ast::block &block) {
            check_block(block, nodedata(stmt.id, stmt.span), ctx);
        },
        [&](const ast::exprstmt &exprstmt) {
            check_exprstmt(exprstmt, nodedata(stmt.id, stmt.span), ctx);
        },
        [&](const ast::var_decl &decl) {
            check_vardecl(decl, nodedata(stmt.id, stmt.span), ctx);
        },
        [&](const ast::return_stmt &return_stmt) {
            check_return_stmt(return_stmt, nodedata(stmt.id, stmt.span), ctx);
        },
        [](std::monostate) {
            UNREACHABLE();
        }
    }, stmt.value);
}

void type_checker::check_return_stmt(const ast::return_stmt &return_stmt, nodedata data, context &ctx) {
    if (return_stmt.value) {
        type_info return_type = check_expr(**return_stmt.value, ctx);
        
        assert(function_context_.has_value());

        function_context fn_ctx = function_context_.value();

        if (return_type.type != fn_ctx.return_type) {
            std::ostringstream oss;
            oss << "returning '";
            oss << return_type.type;
            oss << "' from a function with incompatible result type '";
            oss << fn_ctx.return_type;
            oss << "'";
            error(oss.str(), data.span, ctx);
            return;
        }
    }
}

void type_checker::check_vardecl(const ast::var_decl &decl, nodedata data, context &ctx) {
    localid id = ctx.variables.at(data.id);

    type type_annotation = get_annotation_type(decl.type_annotation);

    if (decl.initialiser == nullptr) {
        ctx.var_types.emplace(id, type_info(type_annotation, value_cat::lvalue));
    } else {
        type_info init_type = check_expr(*decl.initialiser, ctx);

        // check if the type of the expression can be assigned to 
        if (!convertible(init_type.type, type_annotation)) {
            error("type mismatch", data.span, ctx);
            return;
        }


        ctx.var_types.emplace(id, type_info(type_annotation, value_cat::lvalue));
    }
}

bool type_checker::convertible(type from, type target) const {
    if (from == target) {
        return true;
    }

    static_assert(static_cast<int>(type::error) == 2, "fix type checking");

    if (from == type::error || target == type::error) {
        return true;
    }

    switch (from) {
        case type::int_: 
            switch (target) {
                case type::bool_:
                    return false;
                case type::int_:
                    return true;
                case type::error:
                    UNREACHABLE();
                    break;
            }
            break;
        case type::bool_:
            switch (target) {
                case type::int_:
                    return false;
                case type::bool_:
                    return true;
                case type::error:
                    UNREACHABLE();
                    break;
            }
            break;
        case type::error:
            // do nothing because it should be unreachable
            UNREACHABLE();
            break;
    }

    return false;
}

void type_checker::check_block(const ast::block &block, nodedata, context &ctx) {
    for (auto it = block.body.begin(); it != block.body.end(); ++it) {
        check_stmt(**it, ctx);
    }
}

void type_checker::check_exprstmt(const ast::exprstmt &exprstmt, nodedata, context &ctx) {
    check_expr(*exprstmt.expr, ctx);
}

type_info type_checker::check_expr(const ast::expr &expr, context &ctx) {
    type_info res = std::visit(core::overloaded {
        [&](const ast::binary &binary) -> type_info {
            return check_binary(binary, nodedata(expr.id, expr.span), ctx);
        },
        [&](const ast::literal &literal) -> type_info {
            return check_literal(literal, nodedata(expr.id, expr.span), ctx);
        },
        [&](const ast::unary &unary) -> type_info {
            return check_unary(unary, nodedata(expr.id, expr.span), ctx);
        },
        [&](const ast::variable &variable) -> type_info {
            return check_variable(variable, nodedata(expr.id, expr.span), ctx);
        },
        [&](const ast::var_assign &var_assign) -> type_info {
            return check_var_assign(var_assign, nodedata(expr.id, expr.span), ctx);
        },
        [&](const ast::call &call) -> type_info {
            return check_call(call, nodedata(expr.id, expr.span), ctx);
        },
        [](std::monostate) -> type_info {
            UNREACHABLE();
        }
    }, expr.value);

    ctx.types.emplace(expr.id, res);

    return res;
}

type_info type_checker::check_call(const ast::call &call, nodedata data, context &ctx) {
    if (auto it = ctx.function_refs.find(call.callee->id); it != ctx.function_refs.end()) {
        function_info info = ctx.func_info.at(it->second);

        if (info.sig.param_types.size() > call.arguments.size()) {
            error("too few arguments to a function call", data.span, ctx);
            return type_info::error;
        } else if (info.sig.param_types.size() < call.arguments.size()) {
            error("too many arguments to a function call", data.span, ctx);
            return type_info::error;
        }

        for (std::size_t i = 0; i < call.arguments.size(); ++i) {
            type_info arg_type = check_expr(*call.arguments[i], ctx);

            type param_type = info.sig.param_types[i];

            if (!convertible(arg_type.type, param_type)) {
                error("argument type mismatch", call.arguments[i]->span, ctx);
            }
        }

        return type_info(info.sig.return_type, value_cat::rvalue);

    } else {
        error("attempt to use operator '()' on uncallable object", data.span, ctx);
    }
}

type_info type_checker::check_var_assign(const ast::var_assign &var_assign, nodedata data, context &ctx) {
    type_info value_type = check_expr(*var_assign.value, ctx);
    type_info target_type = check_expr(*var_assign.target, ctx);

    if (!convertible(value_type.type, target_type.type)) {
        error("type mismatch", data.span, ctx);
        return type_info::error;
    }

    if (target_type.category == value_cat::rvalue) {
        error("target for assignment expression must be lvalue", data.span, ctx);
        return type_info::error;
    }

    return type_info(target_type.type, value_cat::rvalue);
}

type_info type_checker::check_variable(const ast::variable&, nodedata data, context &ctx) {
    // we use at here because these variables should exist and this is guarenteed by name resolution
    localid id = ctx.variables.at(data.id);
    return ctx.var_types.at(id);
}

type_info type_checker::check_unary(const ast::unary &unary, nodedata data, context &ctx) {
    type_info operand_type = check_expr(*unary.operand, ctx);

    std::optional<type> res = check_unary_type(unary.op.node, operand_type.type);

    if (!res) {
        std::ostringstream oss;
        oss << "unary operator '";
        oss << unary.op.node;
        oss << "' cannot be applied to type: '";
        oss << operand_type.type;
        oss << "'";
        error(oss.str(), data.span, ctx);
        return type_info(type::error, value_cat::invalid); 
    }

    return type_info(*res, value_cat::rvalue);
}

type_info type_checker::check_binary(const ast::binary &binary, nodedata data, context &ctx) {
    type_info left_type = check_expr(*binary.left, ctx);
    type_info right_type = check_expr(*binary.right, ctx);

    std::optional<type> res = check_binary_type(binary.op.node, left_type.type, right_type.type);

    if (!res) {
        std::ostringstream oss;
        oss << "binary operator '";
        oss << binary.op.node;
        oss << "' cannot be applied to types: '";
        oss << left_type.type;
        oss << "' and '";
        oss << right_type.type;
        oss << "'";
        error(oss.str(), data.span, ctx);
        return type_info(type::error, value_cat::invalid);
    }

    return type_info(*res, value_cat::rvalue);
}

type_info type_checker::check_literal(const ast::literal &literal, nodedata data, context &ctx) {
    type_info res = std::visit(core::overloaded {
        [&, this](std::uint64_t integer) -> type_info {
            return check_int_literal(integer, data, ctx);
        },
        [&, this](bool boolean) -> type_info {
            return check_boolean_literal(boolean, data, ctx);
        }
    }, literal.value);

    return res;
}

type_info type_checker::check_boolean_literal(bool, nodedata, context&) {
    return type_info(type::bool_, value_cat::rvalue);
}

type_info type_checker::check_int_literal(std::uint64_t integer, nodedata, context&) {
    if (integer <= std::numeric_limits<std::uint32_t>::max()) {
        return type_info(type::int_, value_cat::rvalue); 
    } else {
        // larger integer types not yet implemented
        UNIMPLEMENTED();
    }
}

std::optional<type> type_checker::check_unary_type(ast::unaryop_type op, type a) const {
    if (a == type::error) {
        return type::error;
    }

    static_assert(static_cast<int>(type::error) == 2, "fix type checking");
    using ast::unaryop_type;

    switch (op) {
        case unaryop_type::logical_not:
            if (a != type::bool_) {
                return std::nullopt;
            }
            return type::bool_;
        case unaryop_type::negate:
            if (a != type::int_) {
                return std::nullopt;
            }
            return type::int_;
    }
}

std::optional<type> type_checker::check_binary_type(ast::binaryop_type op, type lhs, type rhs) const {
    if (lhs == type::error || rhs == type::error) {
        return type::error;
    }

    // i cannot be bothered to type that too many times in the switches
    using ast::binaryop_type;

    static_assert(static_cast<int>(type::error) == 2, "fix type checking");

    switch (op) {
        case binaryop_type::add:
        case binaryop_type::sub:
        case binaryop_type::mul:
        case binaryop_type::div:
            if (lhs == type::int_ && rhs == type::int_) {
                return type::int_;
            } else {
                return std::nullopt;
            }
            break;
        case binaryop_type::equal:
        case binaryop_type::not_equal:
            // currently this is always valid because equality can be aplied on both int and bool
            // and both result in bool
            return type::bool_;
        case binaryop_type::logical_and:
        case binaryop_type::logical_or:
            if (lhs == type::bool_ && rhs == type::bool_) {
                return type::bool_;
            } else {
                return std::nullopt;
            }
        case binaryop_type::greater:
        case binaryop_type::greater_eq:
        case binaryop_type::less:
        case binaryop_type::less_eq:
            if (lhs == type::int_ && rhs == type::int_) {
                return type::bool_;
            } else {
                return std::nullopt;
            }
    }

    return type::int_;
}

type type_checker::get_annotation_type(parser::type_syntax syntax) const {
    using ast::type_syntax_type;

    switch (syntax.type) {
        case type_syntax_type::int_:        
            return type::int_;
        case type_syntax_type::bool_:
            return type::bool_;
        default:
            return type::error;
    }
}
    
} // namespace sema

    
} // namespace cbb
