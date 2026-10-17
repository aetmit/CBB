#ifndef CBB_LEX_TOKEN_HPP
#define CBB_LEX_TOKEN_HPP

#include <iostream>

#include "core/types.hpp"
#include "core/span.hpp"

namespace cbb {

namespace lexer {

enum class TokenType {
    IntegerLiteral,

    OpenParen,
    CloseParen,

    Plus,
    Minus,
    Star,
    Slash,

    Eof,
};

std::ostream &operator<<(std::ostream &os, const TokenType &type);

struct Token {
    TokenType type;
    core::Span span;
    size_t column;
    size_t line;
    Token() = default;
    Token(TokenType type, core::Span span, size_t column, size_t line);


    friend std::ostream &operator<<(std::ostream &os, const Token &token);
};

} // namespace lexer 

} // namespace cbb

#endif // CBB_LEX_TOKEN_HPP