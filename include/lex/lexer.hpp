#ifndef CBB_LEX_LEXER_HPP
#define CBB_LEX_LEXER_HPP

#include "lex/token.hpp"

namespace cbb {

namespace lexer {

class Lexer {
public:
    explicit Lexer(const char *source);
    ~Lexer();
    Token next(); 

private:
    // non owning pointer into source string
    const char *source;
    
    size_t current;
    size_t start;
    size_t column;
    size_t startColumn;
    size_t line;
    size_t sourceLen;

    bool isAtEnd() const;
    void skipWhitespace();
    char advance();
    char peek() const;
    Token newToken(TokenType type) const;
};

} // namespace lexer

} // namespace cbb

#endif // CBB_LEX_LEXER_HPP
