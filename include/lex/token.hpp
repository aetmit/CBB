#ifndef CBB_LEX_TOKEN_HPP
#define CBB_LEX_TOKEN_HPP

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

struct Token {
    TokenType type;
    core::Span span;
    size_t column;
    size_t line;

    Token(TokenType type, core::Span span, size_t column, size_t line);
};

} // namespace lexer 

} // namespace cbb

#endif // CBB_LEX_TOKEN_HPP