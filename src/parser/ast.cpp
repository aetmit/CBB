#include "parser/ast.hpp"

#include <utility>

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

} // namespace parser

} // namespace cbb