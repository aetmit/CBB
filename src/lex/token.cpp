#include "lex/token.hpp"

namespace cbb {

namespace lexer {

Token::Token(TokenType type, core::Span span, size_t column, size_t line): type(type), span(span), column(column), line(line) {}

} // namespace lexer

} // namespace cbb