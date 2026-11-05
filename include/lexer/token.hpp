#ifndef CBB_LEXER_TOKEN_HPP
#define CBB_LEXER_TOKEN_HPP

#include <string_view>

#include <cstddef>

#include "core/span.hpp"

namespace cbb {

namespace lexer {

enum class token_type {
    integer_literal,
    identifier,

    plus,
    minus,
    star,
    slash,

    open_paren,
    close_paren,

    open_brace,
    close_brace,

    equal,

    // keyword int not to be confused with integer literal
    // _ postfix because int is not allowed cuz its the type
    int_,

    semicolon,

    eof,
};

struct token {
    token_type type;
    core::span span;
    std::size_t column;
    std::size_t line;
    
    token(token_type type, core::span span, std::size_t column, std::size_t line);
};
    
} // namespace lexer

    
} // namespace cbb


#endif // CBB_LEXER_TOKEN_HPP