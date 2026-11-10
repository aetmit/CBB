#include "parser/ast.hpp"

#include <sstream>
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

std::string ast_formatter::format(const program &program) {
    std::ostringstream os;
    os << "Program (" << program.body.size() << " statements)\n";

    for (std::size_t index = 0; index < program.body.size(); ++index) {
        write_indent(os, 1);
        os << "statement[" << index << "]:";
        if (program.body[index] == nullptr) {
            os << " <null>\n";
        } else {
            os << "\n";
            format_statement(os, program.body[index], 2);
        }
    }

    return os.str();
}

void ast_formatter::write_indent(std::ostream &os, std::size_t indent) const {
    for (std::size_t level = 0; level < indent; ++level) {
        os << "  ";
    }
}

void ast_formatter::write_span(std::ostream &os, const core::span &span) const {
    os << "offset=" << span.offset() << ", length=" << span.length();
}

void ast_formatter::format_type_syntax(std::ostream &os, const type_syntax &type, std::size_t indent) const {
    write_indent(os, indent);
    os << "type: ";
    switch (type.type) {
        case type_syntax_type::int_:
            os << "int\n";
            return;
    }
    os << "<invalid type>\n";
}

void ast_formatter::format_expression(std::ostream &os, const expr *expression, std::size_t indent) const {
    write_indent(os, indent);
    if (expression == nullptr) {
        os << "<null expression>\n";
        return;
    }

    std::visit([&](const auto &value) {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, std::monostate>) {
            os << "EmptyExpression (id=" << expression->id << ")\n";
            write_indent(os, indent + 1);
            os << "span: ";
            write_span(os, expression->span);
            os << "\n";
        } else if constexpr (std::is_same_v<T, literal>) {
            os << "LiteralExpression (id=" << expression->id << ")\n";
            write_indent(os, indent + 1);
            os << "span: ";
            write_span(os, expression->span);
            os << "\n";
            write_indent(os, indent + 1);
            os << "value: " << value << "\n";
        } else if constexpr (std::is_same_v<T, variable>) {
            os << "VariableExpression (id=" << expression->id << ")\n";
            write_indent(os, indent + 1);
            os << "span: ";
            write_span(os, expression->span);
            os << "\n";
            write_indent(os, indent + 1);
            os << "name: " << value.name << "\n";
        } else if constexpr (std::is_same_v<T, binary>) {
            os << "BinaryExpression (id=" << expression->id << ")\n";
            write_indent(os, indent + 1);
            os << "span: ";
            write_span(os, expression->span);
            os << "\n";
            write_indent(os, indent + 1);
            os << "operator: " << value.op.node << " (";
            write_span(os, value.op.span);
            os << ")\n";
            write_indent(os, indent + 1);
            os << "left:\n";
            format_expression(os, value.left, indent + 2);
            write_indent(os, indent + 1);
            os << "right:\n";
            format_expression(os, value.right, indent + 2);
        } else if constexpr (std::is_same_v<T, unary>) {
            os << "UnaryExpression (id=" << expression->id << ")\n";
            write_indent(os, indent + 1);
            os << "span: ";
            write_span(os, expression->span);
            os << "\n";
            write_indent(os, indent + 1);
            os << "operator: " << value.op.node << " (";
            write_span(os, value.op.span);
            os << ")\n";
            write_indent(os, indent + 1);
            os << "operand:\n";
            format_expression(os, value.operand, indent + 2);
        } else if constexpr (std::is_same_v<T, var_assign>) {
            os << "AssignmentExpression (id=" << expression->id << ")\n";
            write_indent(os, indent + 1);
            os << "span: ";
            write_span(os, expression->span);
            os << "\n";
            write_indent(os, indent + 1);
            os << "target:\n";
            format_expression(os, value.target, indent + 2);
            write_indent(os, indent + 1);
            os << "value:\n";
            format_expression(os, value.value, indent + 2);
        }
    }, expression->value);
}

void ast_formatter::format_statement(std::ostream &os, const stmt *statement, std::size_t indent) const {
    write_indent(os, indent);
    if (statement == nullptr) {
        os << "<null statement>\n";
        return;
    }

    std::visit([&](const auto &value) {
        using T = std::decay_t<decltype(value)>;

        if constexpr (std::is_same_v<T, std::monostate>) {
            os << "EmptyStatement (id=" << statement->id << ")\n";
            write_indent(os, indent + 1);
            os << "span: ";
            write_span(os, statement->span);
            os << "\n";
        } else if constexpr (std::is_same_v<T, exprstmt>) {
            os << "ExpressionStatement (id=" << statement->id << ")\n";
            write_indent(os, indent + 1);
            os << "span: ";
            write_span(os, statement->span);
            os << "\n";
            write_indent(os, indent + 1);
            os << "expression:\n";
            format_expression(os, value.expr, indent + 2);
        } else if constexpr (std::is_same_v<T, var_decl>) {
            os << "VariableDeclaration (id=" << statement->id << ")\n";
            write_indent(os, indent + 1);
            os << "span: ";
            write_span(os, statement->span);
            os << "\n";
            write_indent(os, indent + 1);
            os << "name: " << value.name << "\n";
            format_type_syntax(os, value.type_annotation, indent + 1);
            write_indent(os, indent + 1);
            if (value.initialiser == nullptr) {
                os << "initializer: <none>\n";
            } else {
                os << "initializer:\n";
                format_expression(os, value.initialiser, indent + 2);
            }
        } else if constexpr (std::is_same_v<T, block>) {
            os << "Block (id=" << statement->id << ", " << value.body.size() << " statements)\n";
            write_indent(os, indent + 1);
            os << "span: ";
            write_span(os, statement->span);
            os << "\n";
            for (std::size_t index = 0; index < value.body.size(); ++index) {
                write_indent(os, indent + 1);
                os << "statement[" << index << "]:";
                if (value.body[index] == nullptr) {
                    os << " <null>\n";
                } else {
                    os << "\n";
                    format_statement(os, value.body[index], indent + 2);
                }
            }
        }
    }, statement->value);
}

var_assign::var_assign(expr *target, expr *value): target(target), value(value) {}

} // namespace parser

} // namespace cbb