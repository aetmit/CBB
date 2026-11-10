#include "analysis/type_checker.hpp"

#include <limits>

#include "core/overloaded.hpp"
#include "core/error.hpp"

namespace cbb {

namespace sema {

namespace ast = parser;

void type_checker::check(const ast::program &program, context &ctx) {
    for (auto it = program.body.begin(); it != program.body.end(); ++it) {
        check_stmt(**it, ctx);
    }
}

void type_checker::check_stmt(const ast::stmt &stmt, context &ctx) {
    std::visit(core::overloaded {
        [&, this](const ast::block &block) {
            check_block(block, nodedata(stmt.id, stmt.span), ctx);
        },
        [&, this](const ast::exprstmt &exprstmt) {
            check_exprstmt(exprstmt, nodedata(stmt.id, stmt.span), ctx);
        },
        [&, this](const ast::var_decl &decl) {
            check_vardecl(decl, nodedata(stmt.id, stmt.span), ctx);
        },
        [](std::monostate) {
            UNREACHABLE();
        }
    }, stmt.value);
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

    if (static_cast<int>(type::error) != 2) {
        FATAL("come here and actually fix this");
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
        [&, this](const ast::binary &binary) -> type_info {
            return check_binary(binary, nodedata(expr.id, expr.span), ctx);
        },
        [&, this](const ast::literal &literal) -> type_info {
            return check_literal(literal, nodedata(expr.id, expr.span), ctx);
        },
        [&, this](const ast::unary &unary) -> type_info {
            return check_unary(unary, nodedata(expr.id, expr.span), ctx);
        },
        [&, this](const ast::variable &variable) -> type_info {
            return check_variable(variable, nodedata(expr.id, expr.span), ctx);
        },
        [&, this](const ast::var_assign &var_assign) -> type_info {
            return check_var_assign(var_assign, nodedata(expr.id, expr.span), ctx);
        },
        [](std::monostate) -> type_info {
            UNREACHABLE();
        }
    }, expr.value);

    ctx.types.emplace(expr.id, res);

    return res;
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

    return value_type;
}

type_info type_checker::check_variable(const ast::variable&, nodedata data, context &ctx) {
    // we use at here because these variables should exist and this is guarenteed by name resolution
    localid id = ctx.variables.at(data.id);
    return ctx.var_types.at(id);
}

type_info type_checker::check_unary(const ast::unary &unary, nodedata, context &ctx) {
    type_info operand_type = check_expr(*unary.operand, ctx);

    std::optional<type> res = check_unary_type(unary.op.node, operand_type.type);

    if (!res) {
        FATAL("shouldnt happen");
    }

    return type_info(*res, value_cat::rvalue);
}

type_info type_checker::check_binary(const ast::binary &binary, nodedata, context &ctx) {
    type_info left_type = check_expr(*binary.left, ctx);
    type_info right_type = check_expr(*binary.right, ctx);

    std::optional<type> res = check_binary_type(binary.op.node, left_type.type, right_type.type);

    if (!res) {
        FATAL("shouldnt happen");
    }

    return type_info(*res, value_cat::rvalue);
}

type_info type_checker::check_literal(const ast::literal &literal, nodedata data, context &ctx) {
    type_info res = std::visit(core::overloaded {
        [&, this](std::uint64_t integer) -> type_info {
            return check_int_literal(integer, data, ctx);
        }
    }, literal.value);

    return res;
}

type_info type_checker::check_int_literal(std::uint64_t integer, nodedata, context&) {
    if (integer <= std::numeric_limits<std::uint32_t>::max()) {
        return type_info(type::int_, value_cat::rvalue); 
    } else {
        // larger integer types not yet implemented
        UNIMPLEMENTED();
    }
}

std::optional<type> type_checker::check_unary_type(ast::unaryop_type, type a) const {
    if (a == type::error) {
        return type::error;
    }

    if (static_cast<int>(type::error) != 2) {
        FATAL("someone added new types so actually implement this");
    }

    return type::int_;
}

std::optional<type> type_checker::check_binary_type(ast::binaryop_type, type lhs, type rhs) const {
    if (lhs == type::error || rhs == type::error) {
        return type::error;
    }

    if (static_cast<int>(type::error) != 2) {
        FATAL("just check here and actually implement type checking here when there are actually some more types");
    }
    return type::int_;
}

type type_checker::get_annotation_type(parser::type_syntax syntax) const {
    using ast::type_syntax_type;

    switch (syntax.type) {
        case type_syntax_type::int_:        
            return type::int_;
        default:
            return type::error;
    }
}
    
} // namespace sema

    
} // namespace cbb
