#include "parser/ast.hpp"

namespace cbb {

namespace parser { 

expr::expr(nodeid id, core::span span, literal literal): id(id), type(expr_type::literal), span(span), as{ .literal = literal } {}
expr::expr(nodeid id, core::span span, binary binary): id(id), type(expr_type::binary), span(span), as{ .binary = binary } {}
expr::expr(nodeid id, core::span span, unary unary): id(id), type(expr_type::unary), span(span), as{ .unary = unary } {}
expr::expr(nodeid id, core::span span, variable variable): id(id), type(expr_type::variable), span(span), as{ .variable = variable } {}
expr::expr(nodeid id, core::span span, var_assign var_assign): id(id), type(expr_type::var_assign), span(span), as{ .var_assign = var_assign } {}
expr::expr(nodeid id, core::span span): id(id), type(expr_type::error), span(span), as{ .variable = {} } {}

} // namespace parser

} // namespace cbb