#include "lex/lexer.hpp"

#include <cstring>
#include <cctype>

namespace cbb {

namespace lexer {

using core::Span;

Lexer::Lexer(const char *source): source(source), current(0), start(0), column(0), startColumn(0), line(0) {
    sourceLen = std::strlen(source);
}

Lexer::~Lexer() {
    current = 0;
    start = 0;
    column = 0;
    startColumn = 0;
    line = 0;

    source = nullptr;
}

Token Lexer::next() {
    if (isAtEnd()) {
        return Token(TokenType::Eof, Span(current, 0), column, line);
    }

    skipWhitespace();

    if (isAtEnd()) {
        return Token(TokenType::Eof, Span(current, 0), column, line);
    }

    start = current;
    startColumn = column;

    char c = peek();

    switch (c) {
        case '(':
            advance();
            return newToken(TokenType::OpenParen);
        case ')':
            advance();
            return newToken(TokenType::CloseParen);
        case '+':
            advance();
            return newToken(TokenType::Plus);
        case '-':
            advance();
            return newToken(TokenType::Minus);
        case '*':
            advance();
            return newToken(TokenType::Star);
        case '/':
            advance();
            return newToken(TokenType::Slash);
        default:
            if (std::isdigit(static_cast<unsigned char>(c))) {
                while (std::isdigit(static_cast<unsigned char>(peek()) && !isAtEnd())) {
                    advance();
                }
                return newToken(TokenType::IntegerLiteral);
            } else {
                // unimplemented
                throw 1;
            }
    }
}

Token Lexer::newToken(TokenType type) const {
    return Token(type, Span(start, current - start), startColumn, line);
}

#define EOF_CHAR '\0'

void Lexer::skipWhitespace() {
    while (std::isspace(static_cast<unsigned char>(peek()))) {
        advance();
    }
}

char Lexer::advance() {
    if (isAtEnd()) {
        return EOF_CHAR;
    }

    char c = peek();

    if (c == '\n') {
        line++;
        column = 0;
    } else {
        column++;
    }
    current++;

    return c;
}

char Lexer::peek() const {
    if (isAtEnd()) {
        return EOF_CHAR;
    }
    return source[current];
}

bool Lexer::isAtEnd() const {
    return current >= sourceLen;
}

} // namespace lexer

} // namespace cbb
