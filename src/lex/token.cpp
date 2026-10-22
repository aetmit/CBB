#include "lex/token.hpp"

namespace cbb {

namespace lexer {

Token::Token(TokenType type, core::Span span, size_t column, size_t line): type(type), span(span), column(column), line(line) {}

std::ostream &operator<<(std::ostream &os, const Token &token) {

    os << "Type: " << token.type << "\n";
    os << "Span: " << token.span << "\n";
    os << "Column: " << token.column << "\n";
    os << "Line: " << token.line << "\n";
    
    return os;
}

std::ostream &operator<<(std::ostream &os, const TokenType &type) {
    switch (type) {
        case TokenType::IntegerLiteral: 
            return os << "IntegerLiteral";
        case TokenType::OpenParen:  
            return os << "OpenParen";
        case TokenType::CloseParen: 
            return os << "CloseParen";
        case TokenType::Plus:  
            return os << "Plus";
        case TokenType::Minus: 
            return os << "Minus";
        case TokenType::Star:  
            return os << "Star";
        case TokenType::Slash: 
            return os << "Slash";
        case TokenType::Eof: 
            return os << "Eof";
        case TokenType::OpenBrace:
            return os << "OpenBrace";
        case TokenType::CloseBrace:
            return os << "CloseBrace";
        case TokenType::Identifier:
            return os << "Identifier";
        case TokenType::Int:
            return os << "Int";
        case TokenType::Equal:
            return os << "Equal";
        case TokenType::Semicolon:
            return os << "Semicolon";
    }

    os << "Unknown";

    return os;
}

} // namespace lexer

} // namespace cbb