#ifndef LEX_TOKEN_HPP
#define LEX_TOKEN_HPP

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
};

} // namespace lexer 

} // namespace cbb

#endif // LEX_TOKEN_HPP