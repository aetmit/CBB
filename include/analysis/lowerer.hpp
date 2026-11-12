#ifndef CBB_ANALYSIS_LOWERER_HPP
#define CBB_ANALYSIS_LOWERER_HPP

#include <new>

#include "analysis/ids.hpp"
#include "analysis/hir.hpp"
#include "analysis/context.hpp"
#include "parser/ast.hpp"

namespace cbb {

namespace sema {

class lowerer {
public:
    lowerer();

    program lower(const parser::program &ast, context &ctx);

private:
    hirid current_hirid_;

    // reference to the arena that is owned by the hir program
    std::unique_ptr<core::arena> *arena_;

    stmt *lower_stmt(const parser::stmt &stmt, context &ctx);
    stmt *lower_block(const parser::block &block, context &ctx);
    stmt *lower_exprstmt(const parser::exprstmt &exprstmt, context &ctx);
    stmt *lower_var_decl(const parser::var_decl &var_decl, parser::nodeid nodeid, context &ctx);

    expr *lower_expr(const parser::expr &expr, context &ctx);
    expr *lower_literal(const parser::literal &literal, parser::nodeid id, context &ctx);
    expr *lower_binary(const parser::binary &binary, parser::nodeid id, context &ctx);
    expr *lower_unary(const parser::unary &unary, parser::nodeid id, context &ctx);
    expr *lower_variable(const parser::variable &variable, parser::nodeid id, context &ctx);
    expr *lower_var_assign(const parser::var_assign &var_assign, parser::nodeid id, context &ctx);

    binarytype lower_binaryop(parser::binaryop_type op);
    unarytype lower_unaryop(parser::unaryop_type op);

    hirid next_hirid();

    template <class T> 
    stmt *alloc_stmt(T value) {
        void *memory = (*arena_)->allocate(sizeof(stmt), alignof(stmt));

        stmt *stmt = new (memory) struct stmt(next_hirid(), value);

        return stmt;
    }

    template <class T>
    expr *alloc_expr(type_info type_info, T value) {
        void *memory = (*arena_)->allocate(sizeof(expr), alignof(expr));

        expr *expr = new(memory) struct expr(next_hirid(), type_info, value);

        return expr;
    }

};
    
} // namespace sema

    
} // namespace cbb


#endif // CBB_ANALYSIS_LOWERER_HPP