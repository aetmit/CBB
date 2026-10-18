#ifndef CBB_PARSE_PARSER_HPP
#define CBB_PARSE_PARSER_HPP

#include "parse/ast.hpp"
#include "core/structures/arena.hpp"
#include "lex/lexer.hpp"

namespace cbb {

namespace parser {

class Parser {
public:
    Parser(const char *source);
    ~Parser();
    Program parse();
    
private:    
    // reference to the arena owned by Program
    core::Arena *arena;

    lexer::Lexer tokenstream; 
    // non owning pointer
    const char *source;
};

} // namespace parser

} // namespace cbb

#endif // CBB_PARSE_PARSER_HPP