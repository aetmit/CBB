#include "analysis/hir.hpp"

#include <utility>

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

unary::unary(unarytype op, expr *operand): op(op), operand(operand) {}

binary::binary(binarytype op, expr *left, expr *right): op(op), left(left), right(right) {}

var_assign::var_assign(expr *target, expr *value): target(target), value(value) {}

variable::variable(localid id): id(id) {}

literal::literal(std::int32_t integer): value(integer) {}
    
} // namespace sema

    
} // namespace cbb
