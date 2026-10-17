#include "lex/lexer.hpp"

namespace cbb {

namespace lexer {

Lexer::Lexer(const char *source) {
    this->source = source;

    current = 0;
    start = 0;
    column = 0;
    startColumn = 0;
    line = 0;
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

}

} // namespace lexer

} // namespace cbb
