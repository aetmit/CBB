#include "analysis/hir.hpp"

#include <utility>
#include <sstream>
#include <type_traits>
#include <cstddef>

namespace cbb {

namespace sema {

program::program(std::unique_ptr<core::arena> arena, std::vector<function*> body): arena(std::move(arena)), body(std::move(body)) {}

stmt::stmt(hirid id, exprstmt exprstmt): id(id), value(std::in_place_type<struct exprstmt>, std::move(exprstmt)) {}

stmt::stmt(hirid id, var_decl var_decl): id(id), value(std::in_place_type<struct var_decl>, std::move(var_decl)) {}

stmt::stmt(hirid id, block block): id(id), value(std::in_place_type<struct block>,std::move(block)) {}

stmt::stmt(hirid id, return_stmt return_stmt): id(id), value(std::in_place_type<struct return_stmt>, std::move(return_stmt)) {}

var_decl::var_decl(localid id, type_info type, expr *initaliser): id(id), type(type), initaliser(initaliser) {}

block::block(std::vector<stmt*> body): body(std::move(body)) {}

exprstmt::exprstmt(struct expr *expr): expr(expr) {}

expr::expr(hirid id, type_info type, unary unary): id(id), type(type), value(std::in_place_type<struct unary>, std::move(unary)) {}

expr::expr(hirid id, type_info type, binary binary): id(id), type(type), value(std::in_place_type<struct binary>, std::move(binary)) {}

expr::expr(hirid id, type_info type, var_assign var_assign): id(id), type(type), value(std::in_place_type<struct var_assign>, std::move(var_assign)) {}

expr::expr(hirid id, type_info type, variable variable): id(id), type(type), value(std::in_place_type<struct variable>, std::move(variable)) {}

expr::expr(hirid id, type_info type, literal literal): id(id), type(type), value(std::in_place_type<struct literal>, std::move(literal)) {}

expr::expr(hirid id, type_info type, call call): id(id), type(type), value(std::in_place_type<struct call>, std::move(call)) {}

expr::expr(hirid id, type_info type, function_ref ref): id(id), type(type), value(std::in_place_type<function_ref>, std::move(ref)) {}

unary::unary(unarytype op, expr *operand): op(op), operand(operand) {}

return_stmt::return_stmt(std::optional<expr*> value): value(value) {}

binary::binary(binarytype op, expr *left, expr *right): op(op), left(left), right(right) {}

var_assign::var_assign(expr *target, expr *value): target(target), value(value) {}

variable::variable(localid id): id(id) {}

literal::literal(std::int32_t integer): value(integer) {}

literal::literal(bool value): value(value) {}

call::call(expr *callee, std::vector<expr*> arguments): callee(callee), arguments(std::move(arguments)) {}

function_block::function_block(hirid id, block body): id(id), body(body) {}

function_ref::function_ref(functionid id): id(id) {}

param::param(hirid id, localid local_id, enum type type): id(id), local_id(local_id), type(type) {}

void hir_formatter::format_expr(
    std::ostream& out,
    const expr* node,
    std::size_t depth
) {
    if (!node) {
        indent(out, depth);
        out << "<null>\n";
        return;
    }

    indent(out, depth);

    out << "expr " << node->id
        << " : " << type_name(node->type.type)
        << " (" << value_cat_name(node->type.category) << ")\n";

    std::visit([&](const auto& value) {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, literal>) {
            indent(out, depth + 1);
            out << "literal ";

            std::visit([&](const auto& v) {
                out << v;
            }, value.value);

            out << '\n';
        }
        else if constexpr (std::is_same_v<T, variable>) {
            indent(out, depth + 1);
            out << "variable local " << value.id << '\n';
        }
        else if constexpr (std::is_same_v<T, function_ref>) {
            indent(out, depth + 1);
            out << "function_ref " << value.id << '\n';
        }
        else if constexpr (std::is_same_v<T, var_assign>) {
            indent(out, depth + 1);
            out << "var_assign\n";

            indent(out, depth + 2);
            out << "target:\n";
            format_expr(out, value.target, depth + 3);

            indent(out, depth + 2);
            out << "value:\n";
            format_expr(out, value.value, depth + 3);
        }
        else if constexpr (std::is_same_v<T, binary>) {
            indent(out, depth + 1);
            out << "binary " << binary_name(value.op) << '\n';

            indent(out, depth + 2);
            out << "left:\n";
            format_expr(out, value.left, depth + 3);

            indent(out, depth + 2);
            out << "right:\n";
            format_expr(out, value.right, depth + 3);
        }
        else if constexpr (std::is_same_v<T, unary>) {
            indent(out, depth + 1);
            out << "unary " << unary_name(value.op) << '\n';

            indent(out, depth + 2);
            out << "operand:\n";
            format_expr(out, value.operand, depth + 3);
        }
        else if constexpr (std::is_same_v<T, call>) {
            indent(out, depth + 1);
            out << "call\n";

            indent(out, depth + 2);
            out << "callee:\n";
            format_expr(out, value.callee, depth + 3);

            indent(out, depth + 2);
            out << "arguments:\n";

            for (const expr* argument : value.arguments) {
                format_expr(out, argument, depth + 3);
            }
        }
    }, node->value);
}

// -----------------------------------------------------------------------------
// Statement formatting
// -----------------------------------------------------------------------------

void hir_formatter::format_stmt(
    std::ostream& out,
    const stmt* node,
    std::size_t depth
) {
    if (!node) {
        indent(out, depth);
        out << "<null>\n";
        return;
    }

    indent(out, depth);
    out << "stmt " << node->id << '\n';

    std::visit([&](const auto& value) {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, exprstmt>) {
            indent(out, depth + 1);
            out << "expr_stmt\n";

            format_expr(out, value.expr, depth + 2);
        }
        else if constexpr (std::is_same_v<T, var_decl>) {
            indent(out, depth + 1);
            out << "var_decl\n";

            indent(out, depth + 2);
            out << "local " << value.id
                << " : " << type_name(value.type.type)
                << " (" << value_cat_name(value.type.category) << ")\n";

            indent(out, depth + 2);
            out << "initializer:\n";

            format_expr(out, value.initaliser, depth + 3);
        }
        else if constexpr (std::is_same_v<T, block>) {
            indent(out, depth + 1);
            out << "block\n";

            for (const stmt* child : value.body) {
                format_stmt(out, child, depth + 2);
            }
        }
        else if constexpr (std::is_same_v<T, return_stmt>) {
            indent(out, depth + 1);
            out << "return\n";

            if (value.value.has_value()) {
                format_expr(out, *value.value, depth + 2);
            }
        }
    }, node->value);
}

// -----------------------------------------------------------------------------
// Function formatting
// -----------------------------------------------------------------------------

std::string hir_formatter::format(const program& program) {
    std::ostringstream out;

    out << "program\n";

    for (const function* function : program.body) {
        if (!function) {
            indent(out, 1);
            out << "<null>\n";
            continue;
        }

        indent(out, 1);
        out << "function " << function->id
            << " -> " << type_name(function->return_type) << '\n';

        indent(out, 2);
        out << "parameters:\n";

        for (const param& param : function->params) {
            indent(out, 3);
            out << "param " << param.id
                << " local " << param.local_id
                << " : " << type_name(param.type) << '\n';
        }

        indent(out, 2);
        out << "body:\n";

        if (function->body.has_value()) {
            indent(out, 3);
            out << "function_block " << function->body->id << '\n';

            for (const stmt* child : function->body->body.body) {
                format_stmt(out, child, 4);
            }
        }
        else {
            indent(out, 3);
            out << "<no body>\n";
        }
    }

    return out.str();
}

    
} // namespace sema

    
} // namespace cbb
