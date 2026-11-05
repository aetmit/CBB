#include "lexer/token.hpp"

namespace cbb {

namespace lexer {

token::token(token_type type, core::span span, std::size_t column, std::size_t line): type(type), span(span), column(column), line(line) {}
    
} // namespace lexer

    
} // namespace cbb