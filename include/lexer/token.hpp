#ifndef CBB_LEXER_TOKEN_HPP
#define CBB_LEXER_TOKEN_HPP

#include <string_view>
#include <ostream>

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

    equal_equal,
    bang_equal,

    greater_eq,
    greater,
    less_eq,
    less,

    logical_and,
    logical_or,
    bang,

    return_,

    // _ postfix because we avoid conflict with the c++ keywords
    true_,
    false_,

    // keyword int not to be confused with integer literal
    // _ postfix because int is not allowed cuz its the type
    int_,
    // _ postfix because bool is not alloewd cuz its a type
    bool_,

    semicolon,

    eof,
};

struct token {
    token_type type;
    core::span span;
    std::size_t column;
    std::size_t line;
    
    token(token_type type, core::span span, std::size_t column, std::size_t line);

    bool operator==(const token &other) const;
    bool operator!=(const token &other) const;
};

std::ostream &operator<<(std::ostream &os, const token &token);
std::ostream &operator<<(std::ostream &os, const token_type &type);
    
} // namespace lexer

    
} // namespace cbb


#endif // CBB_LEXER_TOKEN_HPP