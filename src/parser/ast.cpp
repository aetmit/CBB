#include "parser/ast.hpp"

#include <utility>
#include <type_traits>

namespace cbb {

namespace parser { 

expr::expr(nodeid id, core::span span, literal literal): id(id), span(span), value(std::in_place_type<struct literal>, std::move(literal)) {}

expr::expr(nodeid id, core::span span, binary binary): id(id), span(span), value(std::in_place_type<struct binary>, std::move(binary)) {}

expr::expr(nodeid id, core::span span, unary unary): id(id), span(span), value(std::in_place_type<struct unary>, std::move(unary)) {}

expr::expr(nodeid id, core::span span, variable variable): id(id), span(span), value(std::in_place_type<struct variable>, std::move(variable)) {}
 
expr::expr(nodeid id, core::span span, var_assign var_assign): id(id), span(span), value(std::in_place_type<struct var_assign>, std::move(var_assign)) {}
 
expr::expr(nodeid id, core::span span): id(id), span(span), value(std::in_place_type<std::monostate>) {}

stmt::stmt(nodeid id, core::span span, exprstmt exprstmt): id(id), span(span), value(std::in_place_type<struct exprstmt>, std::move(exprstmt)) {}

stmt::stmt(nodeid id, core::span span, var_decl var_decl): id(id), span(span), value(std::in_place_type<struct var_decl>, std::move(var_decl)) {}

stmt::stmt(nodeid id, core::span span, block block): id(id), span(span), value(std::in_place_type<struct block>, std::move(block)) {}

stmt::stmt(nodeid id, core::span span): id(id), span(span), value(std::in_place_type<std::monostate>) {}

var_decl::var_decl(std::string_view name, type_syntax type_annotation, expr *initialiser): 
    name(name), type_annotation(type_annotation), initialiser(initialiser) {}

binary::binary(binop op, expr *left, expr *right): op(op), left(left), right(right) {}

literal::literal(std::uint64_t value): value(std::in_place_type<std::uint64_t>, value) {}

unary::unary(unaryop op, expr *operand): op(op), operand(operand) {}

exprstmt::exprstmt(struct expr *expr): expr(expr) {}

block::block(std::vector<stmt*> body): body(std::move(body)) {}

std::ostream &operator<<(std::ostream &os, const nodeid &id) {
    return os << id.value;
}

std::ostream &operator<<(std::ostream &os, const binaryop_type &op) {
    switch (op) {
        case binaryop_type::add: return os << "+";
        case binaryop_type::sub: return os << "-";
        case binaryop_type::mul: return os << "*";
        case binaryop_type::div: return os << "/";
    }

    return os << "<invalid binary operator>";
}

std::ostream &operator<<(std::ostream &os, const unaryop_type &op) {
    switch (op) {
        case unaryop_type::negate: return os << "-";
    }

    return os << "<invalid unary operator>";
}

std::ostream &operator<<(std::ostream &os, const literal &literal) {
    return std::visit([&](const auto &value) -> std::ostream& {
        return os << value;
    }, literal.value);
}

std::ostream &operator<<(std::ostream &os, const variable &variable) {
    return os << variable.name;
}

std::ostream &operator<<(std::ostream &os, const var_assign &assign) {
    return os << *assign.target << " = " << *assign.value;
}

std::ostream &operator<<(std::ostream &os, const binary &binary) {
    return os << "(" << *binary.left << " "
              << binary.op.node << " "
              << *binary.right << ")";
}

std::ostream &operator<<(std::ostream &os, const unary &unary) {
    return os << unary.op.node << *unary.operand;
}

std::ostream &operator<<(std::ostream &os, const exprstmt &exprstmt) {
    return os << *exprstmt.expr << ";";
}

std::ostream &operator<<(std::ostream &os, const type_syntax &type) {
    switch (type.type) {
        case type_syntax_type::int_:
            return os << "int";
    }

    return os << "<invalid type>";
}

std::ostream &operator<<(std::ostream &os, const var_decl &decl) {
    os << decl.type_annotation << " " << decl.name;

    if (decl.initialiser != nullptr)
        os << " = " << *decl.initialiser;

    return os << ";";
}

std::ostream &operator<<(std::ostream &os, const block &block) {
    os << "{\n";

    for (const stmt *stmt : block.body) {
        os << "    " << *stmt << "\n";
    }

    return os << "}";
}

std::ostream &operator<<(std::ostream &os, const stmt &stmt) {
    return std::visit([&](const auto &value) -> std::ostream& {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, std::monostate>) {
            return os << "<empty statement>";
        } else {
            return os << value;
        }
    }, stmt.value);
}

std::ostream &operator<<(std::ostream &os, const expr &expr) {
    return std::visit([&](const auto &value) -> std::ostream& {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, std::monostate>) {
            return os << "<empty expression>";
        } else {
            return os << value;
        }
    }, expr.value);
}

std::ostream &operator<<(std::ostream &os, const program &program) {
    for (const stmt *stmt : program.body) {
        os << *stmt << "\n";
    }

    return os;
}

} // namespace parser

} // namespace cbb