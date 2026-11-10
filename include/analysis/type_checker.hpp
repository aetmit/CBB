#ifndef CBB_ANALYSIS_TYPES_HPP
#define CBB_ANALYSIS_TYPES_HPP

#include <sstream>
#include <optional>

#include <cstdint>

#include "core/concepts.hpp"
#include "core/diagnostics.hpp"
#include "core/span.hpp"
#include "parser/ast.hpp"
#include "analysis/context.hpp"
#include "analysis/type.hpp"

namespace cbb {

namespace sema {

class type_checker {
public:
    type_checker();

    void check(const parser::program &program, context &ctx);

private:

    template<class T>
    requires core::writable_to_stream<T>
    void error(T message, core::span span, context ctx) {
        std::stringstream ss;
        ss << message;
        ctx.diagnostics.push_back(core::diagnostic::error(ss.str(), span));
    }

    struct nodedata {
        parser::nodeid id;
        core::span span;
        
        constexpr nodedata(parser::nodeid id, core::span span): id(id), span(span) {}
    };

    type get_annotation_type(parser::type_syntax syntax) const;

    bool convertible(type from, type target) const;

    std::optional<type> check_binary_type(parser::binaryop_type op, type lhs, type rhs) const;
    std::optional<type> check_unary_type(parser::unaryop_type op, type a) const;

    void check_stmt(const parser::stmt &stmt, context &ctx);
    void check_block(const parser::block &block, nodedata data, context &ctx);
    void check_exprstmt(const parser::exprstmt &exprstmt, nodedata data, context &ctx);
    void check_vardecl(const parser::var_decl &var_decl, nodedata data, context &ctx);

    type_info check_expr(const parser::expr &expr, context &ctx);
    type_info check_binary(const parser::binary &binary, nodedata data, context &ctx);
    type_info check_literal(const parser::literal &literal, nodedata data, context &ctx);
    type_info check_int_literal(std::uint64_t integer, nodedata data, context &ctx);
    type_info check_unary(const parser::unary &unary, nodedata data, context &ctx);
    type_info check_variable(const parser::variable &variable, nodedata data, context &ctx);
    type_info check_var_assign(const parser::var_assign &var_assign, nodedata data, context &ctx);
};
    
} // namespace sema

    
} // namespace cbb


#endif // CBB_ANALYSIS_TYPES_HPP