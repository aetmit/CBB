#ifndef CBB_PARSER_PARSER_HPP
#define CBB_PARSER_PARSER_HPP

#include <string_view>
#include <new>
#include <utility>

#include "lexer/lexer.hpp"
#include "lexer/token.hpp"
#include "parser/ast.hpp"

namespace cbb {

namespace parser {

class parser {
public:
    parser(std::string_view source);

    program parse();

private:    
    std::string_view source_;

    lexer::lexer tokenstream_;
    lexer::lexer::iterator current_;

    nodeid current_nodeid_; 

    // reference to arena in the program
    core::arena *arena;

    lexer::token peek() const;
    lexer::token advance();

    template<typename... Args>
    expr *alloc_expr(Args... args) {
        void *memory = arena->allocate(sizeof(expr), alignof(expr));

        expr *expr = new (memory) struct expr(std::forward<Args>(args)...);

        return expr;
    }

    stmt *parse_statement();
};
    
} // namespace parser

    
} // namespace cbb


#endif // CBB_PARSER_PARSER_HPP