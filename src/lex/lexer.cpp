#include "lex/lexer.hpp"

#include <cstring>
#include <cctype>

#include "core/types.hpp"
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
        case '{':
            advance();
            return newToken(TokenType::OpenBrace);
        case '}':
            advance();
            return newToken(TokenType::CloseBrace);
        case '=':
            advance();
            return newToken(TokenType::Equal);
        case ';':
            advance();
            return newToken(TokenType::Semicolon);
        default:
            if (std::isdigit(static_cast<unsigned char>(c))) {
                while (!isAtEnd() && std::isdigit(static_cast<unsigned char>(peek()))) {
                    advance();
                }
                return newToken(TokenType::IntegerLiteral);
            } else if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
                char *lexeme = new char[16];
                size_t i = 0;
                size_t cap = 16;
                while (!isAtEnd() && (std::isalnum(static_cast<unsigned char>(peek())) || peek() == '_')) {
                    if (i >= cap) {
                        cap *= 2;
                        char *newLexeme = new char[cap];
                        std::memcpy(newLexeme, lexeme, i);
                        delete[] lexeme;
                        lexeme = newLexeme; 
                    }
                    lexeme[i++] = peek();
                    advance();
                }
                if (i >= cap) {
                    cap += 1;
                    char *newLexeme = new char[cap];
                    std::memcpy(newLexeme, lexeme, i);
                    delete[] lexeme;
                    lexeme = newLexeme;
                }
                lexeme[i] = '\0';

                TokenType identType = getIdentifierType(lexeme);

                delete[] lexeme;

                return newToken(identType);
            } else {
                // unimplemented
                throw 1;
            }
    }
}

TokenType Lexer::getIdentifierType(const char *str) const {
    for (size_t i = 0; i < keywords.size(); i++) {
        if (std::strcmp(str, keywords[i].first) == 0)  {
            return keywords[i].second;
        }
    }
    return TokenType::Identifier;
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
