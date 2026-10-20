#include "parse/ast.hpp"

namespace cbb {

namespace parser {

NodeId::NodeId(size_t value): value(value) {}

Literal::Literal(u64 i): type(LiteralType::Integer), as{ .integer = i } {}

Expr::Expr(NodeId id, core::Span span, BinaryOperator binop): type(ExprType::BinaryOp), id(id), span(span), as{ .binop = binop } {}

Expr::Expr(NodeId id, core::Span span, UnaryOperator unaryop): type(ExprType::UnaryOp), id(id), span(span), as{ .unaryop = unaryop } {}

Expr::Expr(NodeId id, core::Span span, Literal literal): type(ExprType::Literal), id(id), span(span), as{ .literal = literal } {}

Expr::Expr(NodeId id, core::Span span): type(ExprType::Error), id(id), span(span), as{} {}

bool NodeId::operator==(const NodeId &other) const {
    return value == other.value;
}

NodeId &NodeId::operator+=(const size_t &other) {
    this->value += other;
    return *this;
}

NodeId &NodeId::operator++() {
    *this += 1;
    return *this;
}

NodeId::NodeId(): value(0) {}

NodeId NodeId::operator++(int) {
    NodeId tmp = *this;
    ++(*this);
    return tmp;
}

size_t NodeIdHasher::operator()(const NodeId &id) const noexcept {
    return std::hash<size_t>{}(id.value);
}

std::ostream &operator<<(std::ostream &os, const NodeId &nodeid) {
    os << nodeid.value;

    return os;
}

std::ostream &operator<<(std::ostream &os, const ExprType &type) {
    switch (type) {
        case ExprType::BinaryOp:
            os << "BinaryOp";
            break;
        case ExprType::Error:
            os << "Error";
            break;
        case ExprType::Literal:
            os << "Literal";
            break;
        case ExprType::UnaryOp:
            os << "UnaryOp";
            break;
    }

    return os;
}

std::ostream &operator<<(std::ostream &os, const BinaryOpType &type) {
    switch (type) {
        case BinaryOpType::Add:
            os << "Add";
            break;
        case BinaryOpType::Sub:
            os << "Sub";
            break;
        case BinaryOpType::Mul:
            os << "Mul";
            break;
        case BinaryOpType::Div:
            os << "Div";
            break;
    }

    return os;
}

std::ostream &operator<<(std::ostream &os, const UnaryOpType &type) {
    switch (type) {
        case UnaryOpType::Negate:
            os << "Negate";
            break;
    }

    return os;
}

std::ostream &operator<<(std::ostream &os, const BinaryOperator &op) {
    os << "BinaryOp {\n"
       << "    op: " << op.op.node << '\n'
       << "    left: " << *op.left << '\n'
       << "    right: " << *op.right << '\n'
       << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const UnaryOperator &op) {
    os << "UnaryOp {\n"
       << "    op: " << op.op.node << '\n'
       << "    operand: " << *op.operand << '\n'
       << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const Literal &literal) {
    os << "Literal { ";

    switch (literal.type) {
        case LiteralType::Integer:
            os << "integer: " << literal.as.integer;
            break;
    }

    os << " }";

    return os;
}

std::ostream &operator<<(std::ostream &os, const Expr &expr) {
    os << "Expr {\n"
       << "    type: " << expr.type << '\n'
       << "    id: " << expr.id << '\n'
       << "    span: " << expr.span << '\n';

    switch (expr.type) {
        case ExprType::BinaryOp:
            os << "    value: " << expr.as.binop << '\n';
            break;

        case ExprType::UnaryOp:
            os << "    value: " << expr.as.unaryop << '\n';
            break;

        case ExprType::Literal:
            os << "    value: " << expr.as.literal << '\n';
            break;

        case ExprType::Error:
            os << "    value: <error>\n";
            break;
    }

    os << "}";

    return os;
}

std::ostream &operator<<(std::ostream &os, const Program &program) {
    os << "Program {\n"
       << "    base: ";

    if (program.base != nullptr) {
        os << *program.base;
    } else {
        os << "nullptr";
    }

    os << "\n}";

    return os;
}

} // namespace parser

} // namespace cbb