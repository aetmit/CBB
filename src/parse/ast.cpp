#include "parse/ast.hpp"

namespace cbb {

namespace parser {

NodeId::NodeId(size_t value): value(value) {}

Literal::Literal(u64 i): type(LiteralType::Integer), as{ .integer = i } {}

Expr::Expr(NodeId id, core::Span span, BinaryOperator binop): type(ExprType::BinaryOp), id(id), span(span), as{ .binop = binop } {}

Expr::Expr(NodeId id, core::Span span, UnaryOperator unaryop): type(ExprType::UnaryOp), id(id), span(span), as{ .unaryop = unaryop } {}

Expr::Expr(NodeId id, core::Span span, Literal literal): type(ExprType::Literal), id(id), span(span), as{ .literal = literal } {}

Expr::Expr(NodeId id, core::Span span): type(ExprType::Error), id(id), span(span), as{} {}

} // namespace parser

} // namespace cbb