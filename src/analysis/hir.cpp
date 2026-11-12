#include "analysis/hir.hpp"

#include <utility>
#include <sstream>
#include <type_traits>
#include <cstddef>

namespace cbb {

namespace sema {

program::program(std::unique_ptr<core::arena> arena, std::vector<stmt*> body): arena(std::move(arena)), body(std::move(body)) {}

stmt::stmt(hirid id, exprstmt exprstmt): id(id), value(std::in_place_type<struct exprstmt>, std::move(exprstmt)) {}

stmt::stmt(hirid id, var_decl var_decl): id(id), value(std::in_place_type<struct var_decl>, std::move(var_decl)) {}

stmt::stmt(hirid id, block block): id(id), value(std::in_place_type<struct block>,std::move(block)) {}

var_decl::var_decl(localid id, type_info type, expr *initaliser): id(id), type(type), initaliser(initaliser) {}

block::block(std::vector<stmt*> body): body(std::move(body)) {}

exprstmt::exprstmt(struct expr *expr): expr(expr) {}

expr::expr(hirid id, type_info type, unary unary): id(id), type(type), value(std::in_place_type<struct unary>, std::move(unary)) {}

expr::expr(hirid id, type_info type, binary binary): id(id), type(type), value(std::in_place_type<struct binary>, std::move(binary)) {}

expr::expr(hirid id, type_info type, var_assign var_assign): id(id), type(type), value(std::in_place_type<struct var_assign>, std::move(var_assign)) {}

expr::expr(hirid id, type_info type, variable variable): id(id), type(type), value(std::in_place_type<struct variable>, std::move(variable)) {}

expr::expr(hirid id, type_info type, literal literal): id(id), type(type), value(std::in_place_type<struct literal>, std::move(literal)) {}

unary::unary(unarytype op, expr *operand): op(op), operand(operand) {}

binary::binary(binarytype op, expr *left, expr *right): op(op), left(left), right(right) {}

var_assign::var_assign(expr *target, expr *value): target(target), value(value) {}

variable::variable(localid id): id(id) {}

literal::literal(std::int32_t integer): value(integer) {}

void hir_formatter::indent(std::ostream &out, std::size_t depth) {
    for (std::size_t i = 0; i < depth; ++i) {
        out << "  ";
    }
}

const char *hir_formatter::type_name(type t) {
    switch (t) {
        case type::int_: return "int";
        case type::error: return "error";
    }

    return "unknown";
}

const char *hir_formatter::value_cat_name(value_cat cat) {
    switch (cat) {
        case value_cat::lvalue: return "lvalue";
        case value_cat::rvalue: return "rvalue";
        case value_cat::invalid: return "invalid";
    }

    return "unknown";
}

const char *hir_formatter::binary_name(binarytype op) {
    switch (op) {
        case binarytype::add: return "add";
        case binarytype::sub: return "sub";
        case binarytype::mul: return "mul";
        case binarytype::div: return "div";
    }

    return "unknown";
}

const char *hir_formatter::unary_name(unarytype op) {
    switch (op) {
        case unarytype::negate: return "negate";
    }

    return "unknown";
}

void hir_formatter::format_expr(
    std::ostream &out,
    const expr *node,
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

    std::visit([&](const auto &value) {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, literal>) {
            indent(out, depth + 1);
            out << "literal ";

            std::visit([&](const auto &v) {
                out << v;
            }, value.value);

            out << '\n';
        }
        else if constexpr (std::is_same_v<T, variable>) {
            indent(out, depth + 1);
            out << "variable local " << value.id << '\n';
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
    }, node->value);
}

void hir_formatter::format_stmt(
    std::ostream &out,
    const stmt *node,
    std::size_t depth
) {
    if (!node) {
        indent(out, depth);
        out << "<null>\n";
        return;
    }

    indent(out, depth);
    out << "stmt " << node->id << '\n';

    std::visit([&](const auto &value) {
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

            for (const stmt *child : value.body) {
                format_stmt(out, child, depth + 2);
            }
        }
    }, node->value);
}

std::string hir_formatter::format(const program &program) {
    std::ostringstream out;

    out << "program\n";

    for (const stmt *node : program.body) {
        format_stmt(out, node, 1);
    }

    return out.str();
}
    
} // namespace sema

    
} // namespace cbb
