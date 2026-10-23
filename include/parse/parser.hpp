#ifndef CBB_PARSE_PARSER_HPP
#define CBB_PARSE_PARSER_HPP

#include <vector>

#include "parse/ast.hpp"
#include "core/diagnostics.hpp"
#include "core/span.hpp"
#include "core/types.hpp"
#include "core/arena.hpp"
#include "lex/lexer.hpp"

namespace cbb {

namespace parser {

class Parser {
public:
    Parser(const char *source, std::vector<core::Diagnostic> *diagnostics);
    ~Parser();
    Program parse();
    
private:    
    // reference to the arena owned by Program
    core::arena *arena;

    lexer::Lexer tokenstream; 
    // non owning pointer
    const char *source;

    // also non owning
    std::vector<core::Diagnostic> *diagnostics;

    NodeId currentNodeId;
    NodeId nextNodeId();

    lexer::Token current;

    void error(const char *message, bool isAllocated, core::Span);

    void sync();

    // parsing functions
    lexer::Token peek() const;
    lexer::Token advance();

    Expr *parseExpression();
    Expr *parseTerm();
    Expr *parseFactor();
    Expr *parseUnary();
    Expr *parsePrimary();

    Stmt *parseStatement();
    Stmt *parseExprStmt();
};

} // namespace parser

} // namespace cbb

#endif // CBB_PARSE_PARSER_HPP