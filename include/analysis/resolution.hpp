#ifndef CBB_ANALYSIS_RESOLUTION_HPP
#define CBB_ANALYSIS_RESOLUTION_HPP

#include <sstream>
#include <string_view>
#include <optional>

#include "core/span.hpp"
#include "core/concepts.hpp"
#include "analysis/ids.hpp"
#include "analysis/context.hpp"
#include "parser/ast.hpp"

namespace cbb {

namespace sema {

class resolver {
public:
    resolver();

    void resolve(const parser::program &program, context &ctx);
private:
    localid current_local_id_;

    template<class T>
    requires core::writable_to_stream<T>
    void error(T message, core::span span, context &ctx) {
        std::stringstream ss;
        ss << message;
        ctx.diagnostics.push_back(core::diagnostic::error(ss.str(), span));
    } 

    localid declare_var(std::string_view name, core::span span, context &ctx);
    std::optional<localid> lookup_var(std::string_view name, context &ctx);
    
    void enter_scope(context &ctx);
    void exit_scope(context &ctx);

    struct nodedata {
        core::span span;
        parser::nodeid id;
        nodedata(parser::nodeid id, core::span span);
    };

    void resolve_stmt(const parser::stmt &stmt, context &ctx);
    void resolve_var_decl(const parser::var_decl &var_decl, nodedata data, context &ctx);
    void resolve_block(const parser::block &block, nodedata data, context &ctx);
    void resolve_exprstmt(const parser::exprstmt &exprstmt, nodedata data, context &ctx);

    void resolve_expr(const parser::expr &expr, context &ctx);
    void resolve_literal(const parser::literal &literal, nodedata data, context &ctx);
    void resolve_var_assign(const parser::var_assign &var_assign, nodedata data, context &ctx);
    void resolve_variable(const parser::variable &variable, nodedata data, context &ctx);
    void resolve_binary(const parser::binary &binary, nodedata data, context &ctx);
    void resolve_unary(const parser::unary &unary, nodedata data, context &ctx);
};
    
} // namespace sema

    
} // namespace cbb


#endif // CBB_ANALYSIS_RESOLUTION_HPP