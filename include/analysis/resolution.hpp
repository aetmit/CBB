#ifndef CBB_ANALYSIS_RESOLUTION_HPP
#define CBB_ANALYSIS_RESOLUTION_HPP

#include <sstream>
#include <string_view>
#include <optional>
#include <vector>

#include "core/span.hpp"
#include "analysis/ids.hpp"
#include "analysis/context.hpp"
#include "analysis/function_info.hpp"
#include "analysis/type.hpp"
#include "parser/ast.hpp"

namespace cbb {

namespace sema {

class resolver {
public:
    resolver();

    void resolve(const parser::program &program, context &ctx);
private:
    localid current_local_id_;
    functionid current_function_id_;

    // true if we are checking a function rn 
    // false if not
    // we use this to check if return is placed inside a function
    bool in_fn_;

    template <class T>
    void error(T message, core::span span, context &ctx) {
        std::stringstream ss;
        ss << message;
        ctx.diagnostics.push_back(core::diagnostic::error(ss.str(), span));
    } 

    localid declare_var(std::string_view name, core::span span, context &ctx);
    std::optional<localid> lookup_var(std::string_view name, context &ctx);

    functionid declare_func(const parser::function &fn, context &ctx);
    std::optional<functionid> lookup_func(std::string_view name, context &ctx);
    
    void enter_scope(context &ctx);
    void exit_scope(context &ctx);

    signature get_fn_signature(const parser::function &fn) const;
    type resolve_type(const parser::type_syntax &type) const;

    void declare_fn_params(const std::vector<parser::param> &params, context &ctx);

    struct nodedata {
        core::span span;
        parser::nodeid id;
        nodedata(parser::nodeid id, core::span span);
    };

    void resolve_function(const parser::function &function, context &ctx);

    void resolve_stmt(const parser::stmt &stmt, context &ctx);
    void resolve_var_decl(const parser::var_decl &var_decl, nodedata data, context &ctx);
    void resolve_block(const parser::block &block, nodedata data, context &ctx);
    void resolve_exprstmt(const parser::exprstmt &exprstmt, nodedata data, context &ctx);
    void resolve_return_stmt(const parser::return_stmt &return_stmt, nodedata data, context &ctx);
    void resolve_if_stmt(const parser::if_stmt &if_stmt, nodedata data, context &ctx);

    void resolve_expr(const parser::expr &expr, context &ctx);
    void resolve_literal(const parser::literal &literal, nodedata data, context &ctx);
    void resolve_var_assign(const parser::var_assign &var_assign, nodedata data, context &ctx);
    void resolve_variable(const parser::variable &variable, nodedata data, context &ctx);
    void resolve_binary(const parser::binary &binary, nodedata data, context &ctx);
    void resolve_unary(const parser::unary &unary, nodedata data, context &ctx);
    void resolve_call(const parser::call &call, nodedata data, context &ctx);
};
    
} // namespace sema

    
} // namespace cbb


#endif // CBB_ANALYSIS_RESOLUTION_HPP