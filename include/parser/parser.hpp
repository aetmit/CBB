#ifndef CBB_PARSER_PARSER_HPP
#define CBB_PARSER_PARSER_HPP

#include <string_view>
#include <new>
#include <sstream>
#include <concepts>
#include <expected>
#include <memory>
#include <vector>

#include "core/concepts.hpp"
#include "core/diagnostics.hpp"
#include "lexer/lexer.hpp"
#include "lexer/token.hpp"
#include "parser/ast.hpp"

namespace cbb {

namespace parser {

class parser {
public:
    parser(std::string_view source, std::vector<core::diagnostic> &diagnostics);

    program parse();

private:    
    std::string_view source_;

    lexer::lexer tokenstream_;
    lexer::lexer::iterator current_;

    nodeid current_nodeid_; 
    functionid current_functionid_;

    // reference to arena in the program
    std::unique_ptr<core::arena> *arena_;

    std::vector<core::diagnostic> &diagnostics_;

    lexer::token peek() const;
    lexer::token advance();

    nodeid next_nodeid();
    functionid next_functionid();

    void sync();

    template <class T>
    expr *alloc_expr(core::span span, T value) {
        void *memory = (*arena_)->allocate(sizeof(expr), alignof(expr));

        expr *expr = new (memory) struct expr(next_nodeid(), span, value);

        return expr;
    }

    expr *alloc_expr(core::span span);

    template <class T>
    stmt *alloc_stmt(core::span span, T value) {
        void *memory = (*arena_)->allocate(sizeof(stmt), alignof(stmt));

        stmt *stmt = new(memory) struct stmt(next_nodeid(), span, value);
        
        return stmt;
    }

    function *alloc_function(core::span span, std::string_view name, type_syntax return_type, std::optional<function_block> body, std::vector<param> params);
    function *alloc_function(core::span span);

    template <class T>
    requires core::writable_to_stream<T>
    void error(T msg, core::span span) {
        std::stringstream ss;
        ss << msg;
        diagnostics_.push_back(core::diagnostic::error(ss.str(), span));
    }

    stmt *alloc_stmt(core::span span);

    function *parse_function();
    std::expected<param, core::span> parse_param();
    std::expected<function_block, core::span> parse_function_body();

    stmt *parse_statement();
    stmt *parse_stmtexpr();
    stmt *parse_vardecl();
    stmt *parse_block();
    stmt *parse_return();
    
    type_syntax parse_type();
    bool is_builtin_type(lexer::token_type type) const;
    type_syntax_type get_builtin_type() const;
    bool is_typename(lexer::token_type type) const;

    // checks if the token the parser is pointing to can be used to start an expression
    bool is_expression_starter() const;

    expr *parse_expression();
    expr *parse_assignment();
    expr *parse_or();
    expr *parse_and();
    expr *parse_equality();
    expr *parse_comparison();
    expr *parse_term();
    expr *parse_factor();
    expr *parse_unary();
    expr *parse_postfix();
    expr *parse_primary();
};
    
} // namespace parser

    
} // namespace cbb


#endif // CBB_PARSER_PARSER_HPP